#include "..\include\minimidi.h"

#include <assert.h>

#include <iostream>
#include <streambuf>

using namespace MiniMidi;
using namespace std;

namespace
{
	enum class EStatus
	{
		Running,
		Success,
		Error,
	};

	struct SReadData
	{
		uint8_t RunningStatus = 0;
		bool bEndOfTrack = false;
		EStatus Status = EStatus::Running;
		std::string ErrorMessage;
	};

	void SetError(SReadData& ReadData, const char* pErrorMessage)
	{
		ReadData.Status = EStatus::Error;
		ReadData.ErrorMessage = pErrorMessage;
	}

	template<typename T>
	T SwapBytes(T Value)
	{
		assert(0);
	}

	template<>
	uint16_t SwapBytes<uint16_t>(uint16_t Value)
	{
		return ((Value & 0xff) << 8) | ((Value & 0xff00) >> 8);
	}

	template<>
	uint32_t SwapBytes<uint32_t>(uint32_t Value)
	{
		return ((Value >> 24) & 0x000000FF) | ((Value >> 8) & 0x0000FF00) | ((Value << 8) & 0x00FF0000) | ((Value << 24) & 0xFF000000);
	}

	template<typename T>
	T Read(std::istream& Stream)
	{
		T Value;
		Stream.read((char*)&Value, sizeof(T));
		return Value;
	}

	bool ReadHeader(SReadData& ReadData, std::istream& Stream, IMidiReader& MidiReader)
	{
		uint32_t Length = SwapBytes(Read<uint32_t>(Stream));

		if (Length < 6)
		{
			SetError(ReadData, "Corrupt midi header");
			return false;
		}

		uint16_t Format = SwapBytes(Read<uint16_t>(Stream));
		uint16_t Tracks = SwapBytes(Read<uint16_t>(Stream));
		uint16_t Division = SwapBytes(Read<uint16_t>(Stream));

		MidiReader.OnHeader(Format, Tracks, Division);

		return true;
	}

	uint32_t ReadVariableLength(SReadData& ReadData, std::istream& Stream)
	{
		uint32_t Value = 0;
		uint8_t CurrentByte = 0;
		do
		{
			CurrentByte = Read<uint8_t>(Stream);
			Value = (Value << 7) + CurrentByte & 0x7f;

		} while (CurrentByte & 0x80);

		return Value;
	}

	bool ReadMetaEvent(SReadData& ReadData, std::istream& Stream, IMidiReader& MidiReader)
	{
		uint8_t Type = Read<uint8_t>(Stream);
		uint32_t Length = ReadVariableLength(ReadData, Stream);

		if (Type == 0x2F)
		{
			ReadData.bEndOfTrack = true;
		}

		Stream.ignore(Length);

		return true;
	}

	bool ReadSysexEvent(SReadData& ReadData, std::istream& Stream, IMidiReader& MidiReader)
	{
		uint32_t Length = ReadVariableLength(ReadData, Stream);

		Stream.ignore(Length);

		return true;
	}

	bool ReadSysexEscape(SReadData& ReadData, std::istream& Stream, IMidiReader& MidiReader)
	{
		uint32_t Length = ReadVariableLength(ReadData, Stream);

		Stream.ignore(Length);

		return true;
	}

	bool ReadMidiEvent(SReadData& ReadData, std::istream& Stream, uint8_t Status, IMidiReader& MidiReader)
	{
		uint8_t Command = Status & 0xF0;
		uint8_t Channel = Status & 0x0F;

		switch (Command)
		{
		case 0x80:
		{
			uint8_t A = Read<uint8_t>(Stream);
			uint8_t B = Read<uint8_t>(Stream);
		}
			break;
		case 0x90:
		{
			uint8_t A = Read<uint8_t>(Stream);
			uint8_t B = Read<uint8_t>(Stream);
		}
			break;
		case 0xA0:
		{
			uint8_t A = Read<uint8_t>(Stream);
			uint8_t B = Read<uint8_t>(Stream);
		}
			break;
		case 0xB0:
		{
			uint8_t A = Read<uint8_t>(Stream);
			uint8_t B = Read<uint8_t>(Stream);
		}
			break;
		case 0xC0:
		{
			uint8_t A = Read<uint8_t>(Stream);
		}
			break;
		case 0xD0:
		{
			uint8_t A = Read<uint8_t>(Stream);
		}
			break;
		case 0xE0:
		{
			uint8_t A = Read<uint8_t>(Stream);
			uint8_t B = Read<uint8_t>(Stream);
		}
			break;
		default:
			SetError(ReadData, "Invalid midi event encountered");
			return false;
		}

		return true;
	}

	bool ReadTimeEventTuple(SReadData& ReadData, std::istream& Stream, IMidiReader& MidiReader)
	{
		uint32_t DeltaTime = ReadVariableLength(ReadData, Stream);
		uint8_t Status = Read<uint8_t>(Stream);

		if (!(Status & 0x80))
		{
			Status = ReadData.RunningStatus;
			Stream.seekg(-1, std::ios_base::cur);
		}

		ReadData.RunningStatus = Status;

		if (Status == 0xFF)
		{
			return ReadMetaEvent(ReadData, Stream, MidiReader);
		}
		else if (Status == 0xF0)
		{
			return ReadSysexEvent(ReadData, Stream, MidiReader);
		}
		else if (Status == 0xF7)
		{
			return ReadSysexEscape(ReadData, Stream, MidiReader);
		}
		
		return ReadMidiEvent(ReadData, Stream, Status, MidiReader);
	}

	bool ReadTrack(SReadData& ReadData, std::istream& Stream, IMidiReader& MidiReader)
	{
		uint32_t Length = SwapBytes(Read<uint32_t>(Stream));

		if (Length < 1)
		{
			SetError(ReadData, "Corrupt track encountered");
			return false;
		}

		MidiReader.OnTrack(Length);

		while (!Stream.eof() && !ReadData.bEndOfTrack)
		{
			if (!ReadTimeEventTuple(ReadData, Stream, MidiReader))
			{
				SetError(ReadData, "Failed to read event");
				return false;
			}
		}

		ReadData.bEndOfTrack = false;

		return true;
	}

	bool ReadChunk(SReadData& ReadData, std::istream& Stream, IMidiReader& MidiReader)
	{
		uint32_t Type = Read<uint32_t>(Stream);

		if (Stream.eof())
		{
			if (ReadData.Status == EStatus::Running)
			{
				ReadData.Status = EStatus::Success;
			}

			return false;
		}

		switch (Type)
		{
		case 1684558925: // MThd
		{
			return ReadHeader(ReadData, Stream, MidiReader);
		}
		case 1802654797: // MTrk
		{
			return ReadTrack(ReadData, Stream, MidiReader);
		}
		default:
			SetError(ReadData, "Invalid chunk type encountered");
			return false;
		}

		return true;
	}

	bool ReadMidi(std::istream& Stream, IMidiReader& MidiReader)
	{
		SReadData ReadData;
		while (ReadChunk(ReadData, Stream, MidiReader))
			;

		if (ReadData.Status == EStatus::Running)
		{
			SetError(ReadData, "Finished reading with an unhandled error");
		}
		else if (ReadData.Status == EStatus::Success)
		{
			MidiReader.OnSuccess();
			return true;
		}
		else if (ReadData.Status == EStatus::Error)
		{
			MidiReader.OnError(ReadData.ErrorMessage.c_str());
			return false;
		}

		assert(0);
		return false;
	}

	struct MemoryBuffer : std::streambuf
	{
		MemoryBuffer(const char* pBegin, const char* pEnd) 
		{
			setg((char*)pBegin, (char*)pBegin, (char*)pEnd);
		}
	};
}

namespace MiniMidi
{
	bool ReadData(const char* pData, int DataLength, IMidiReader& MidiReader)
	{
		MemoryBuffer Buffer(pData, pData + DataLength);
		std::istream Stream(&Buffer, ios::in | ios::binary);
		return ReadMidi(Stream, MidiReader);
	}

	bool ReadData(std::istream& Stream, IMidiReader& MidiReader)
	{
		return ReadMidi(Stream, MidiReader);
	}
}
