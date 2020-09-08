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
		uint16_t NumReadTracks = 0;
		uint16_t ExpectingNumTracks = 0;
		uint8_t RunningStatus = 0;
		bool bEndOfTrack = false;
		EStatus Status = EStatus::Running;
		std::string ErrorMessage;
		bool bReadHeader = false;
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

	bool ReadHeader(SReadData& ReadData, uint32_t Length, std::istream& Stream, IMidiReader& MidiReader)
	{
		if (Length < 6)
		{
			SetError(ReadData, "Corrupt midi header");
			return false;
		}

		uint16_t Format = SwapBytes(Read<uint16_t>(Stream));
		uint16_t Tracks = ReadData.ExpectingNumTracks = SwapBytes(Read<uint16_t>(Stream));
		uint16_t Division = SwapBytes(Read<uint16_t>(Stream));

		ReadData.bReadHeader = true;

		MidiReader.OnHeader(Format, Tracks, Division);

		return true;
	}

	uint32_t ReadVariableLength(std::istream& Stream)
	{
		uint32_t Count = 0;
		uint32_t Value = 0;
		uint8_t CurrentByte = 0;
		do
		{
			CurrentByte = Read<uint8_t>(Stream);
			Value = (Value << 7) + (CurrentByte & 0x7f);
			Count++;

			if (Count > 20)
				throw 20;

		} while (CurrentByte & 0x80);

		return Value;
	}

	char loc_TextBuffer[32768];
	void ReadToTextbuffer(std::istream& Stream, uint32_t Length)
	{
		Stream.read(loc_TextBuffer, Length);
		loc_TextBuffer[Length] = '\0';
	}

	bool ReadMetaEvent(SReadData& ReadData, std::istream& Stream, uint32_t DeltaTime, IMidiReader& MidiReader)
	{
		EMetaEvent Type = (EMetaEvent)Read<uint8_t>(Stream);
		uint32_t Length = ReadVariableLength(Stream);

		assert(Length < 32768);

		switch (Type)
		{
		case EMetaEvent::EndOfTrack:
		{
			ReadData.bEndOfTrack = true;
			MidiReader.OnEndOfTrack(DeltaTime);
		}
		break;
		case EMetaEvent::SequenceNumber:
		{
			uint16_t Number = Read<uint16_t>(Stream);
			MidiReader.OnSequenceNumber(DeltaTime, Number);
		}
		break;
		case EMetaEvent::TextEvent:
		{
			ReadToTextbuffer(Stream, Length);
			MidiReader.OnTextEvent(DeltaTime, loc_TextBuffer);
		}
		break;
		case EMetaEvent::CopyrightNotice:
		{
			ReadToTextbuffer(Stream, Length);
			MidiReader.OnCopyrightNotice(DeltaTime, loc_TextBuffer);
		}
		break;
		case EMetaEvent::TrackName:
		{
			ReadToTextbuffer(Stream, Length);
			MidiReader.OnTrackName(DeltaTime, loc_TextBuffer);
		}
		break;
		case EMetaEvent::InstrumentName:
		{
			ReadToTextbuffer(Stream, Length);
			MidiReader.OnInstrumentName(DeltaTime, loc_TextBuffer);
		}
		break;
		case EMetaEvent::Lyric:
		{
			ReadToTextbuffer(Stream, Length);
			MidiReader.OnLyric(DeltaTime, loc_TextBuffer);
		}
		break;
		case EMetaEvent::Marker:
		{
			ReadToTextbuffer(Stream, Length);
			MidiReader.OnMarker(DeltaTime, loc_TextBuffer);
		}
		break;
		case EMetaEvent::CuePoint:
		{
			ReadToTextbuffer(Stream, Length);
			MidiReader.OnCuePoint(DeltaTime, loc_TextBuffer);
		}
		break;
		case EMetaEvent::ChannelPrefix:
		{
			uint8_t Channel = Read<uint8_t>(Stream);
			MidiReader.OnChannelPrefix(DeltaTime, Channel);
		}
		break;
		case EMetaEvent::MidiPort:
		{
			uint8_t Port = Read<uint8_t>(Stream);
			MidiReader.OnMidiPort(DeltaTime, Port);
		}
		break;
		case EMetaEvent::SetTempo:
		{
			uint8_t A = Read<uint8_t>(Stream);
			uint8_t B = Read<uint8_t>(Stream);
			uint8_t C = Read<uint8_t>(Stream);
			uint32_t Tempo = (A << 16) | (B << 8) | C;
			MidiReader.OnSetTempo(DeltaTime, Tempo);
		}
		break;
		case EMetaEvent::SMPTEOffset:
		{
			uint8_t Hour = Read<uint8_t>(Stream);
			uint8_t Minute = Read<uint8_t>(Stream);
			uint8_t Second = Read<uint8_t>(Stream);
			uint8_t Frame = Read<uint8_t>(Stream);
			uint8_t FrameFraction = Read<uint8_t>(Stream);
			MidiReader.OnSMPTEOffset(DeltaTime, Hour, Minute, Second, Frame, FrameFraction);
		}
		break;
		case EMetaEvent::TimeSignature:
		{
			uint8_t Numerator = Read<uint8_t>(Stream);
			uint8_t Denominator = Read<uint8_t>(Stream);
			uint8_t MidiClocksPerMetronome = Read<uint8_t>(Stream);
			uint8_t Num32thPer24Midi = Read<uint8_t>(Stream);
			MidiReader.OnTimeSignature(DeltaTime, Numerator, Denominator, MidiClocksPerMetronome, Num32thPer24Midi);
		}
		break;
		case EMetaEvent::KeySignature:
		{
			int8_t SharpsOrFlats = Read<int8_t>(Stream);
			uint8_t MajorMinor = Read<uint8_t>(Stream);
			MidiReader.OnKeySignature(DeltaTime, SharpsOrFlats, MajorMinor == 1);
		}
		break;
		case EMetaEvent::SequencerSpecific:
		{
			ReadToTextbuffer(Stream, Length);
			MidiReader.OnSequencerSpecific(DeltaTime, Length, loc_TextBuffer);
		}
		break;
		default:
		{
			ReadToTextbuffer(Stream, Length);
			MidiReader.OnUnhandledMetaEvent(DeltaTime, (uint8_t)Type, Length, loc_TextBuffer);
		}
		break;
		}

		return true;
	}

	bool ReadSysexEvent(SReadData& ReadData, std::istream& Stream, uint32_t DeltaTime, IMidiReader& MidiReader)
	{
		uint32_t Length = ReadVariableLength(Stream);
		ReadToTextbuffer(Stream, Length);
		MidiReader.OnSysexEvent(DeltaTime, Length, loc_TextBuffer);
		ReadData.RunningStatus = 0;

		return true;
	}

	bool ReadSysexEscape(SReadData& ReadData, std::istream& Stream, uint32_t DeltaTime, IMidiReader& MidiReader)
	{
		uint32_t Length = ReadVariableLength(Stream);
		ReadToTextbuffer(Stream, Length);
		MidiReader.OnSysexEscape(DeltaTime, Length, loc_TextBuffer);
		ReadData.RunningStatus = 0;

		return true;
	}

	bool ReadMidiEvent(SReadData& ReadData, std::istream& Stream, uint8_t Status, uint32_t DeltaTime, IMidiReader& MidiReader)
	{
		EMidiMessage Message = (EMidiMessage)(Status & 0xF0);
		uint8_t Channel = Status & 0x0F;

		switch (Message)
		{
		case EMidiMessage::NoteOff:
		{
			uint8_t Key = Read<uint8_t>(Stream);
			uint8_t Velocity = Read<uint8_t>(Stream);
			MidiReader.OnNoteOff(DeltaTime, Key, Velocity);
		}
		break;
		case EMidiMessage::NoteOn:
		{
			uint8_t Key = Read<uint8_t>(Stream);
			uint8_t Velocity = Read<uint8_t>(Stream);
			if(Velocity == 0)
				MidiReader.OnNoteOff(DeltaTime, Key, Velocity);
			else
				MidiReader.OnNoteOn(DeltaTime, Key, Velocity);
		}
		break;
		case EMidiMessage::PolyphonicKeyPressure:
		{
			uint8_t Key = Read<uint8_t>(Stream);
			uint8_t Pressure = Read<uint8_t>(Stream);
			MidiReader.OnPolyphonicKeyPressure(DeltaTime, Key, Pressure);
		}
		break;
		case EMidiMessage::ControlChange:
		{
			uint8_t ControllerNumber = Read<uint8_t>(Stream);
			uint8_t Value = Read<uint8_t>(Stream);
			MidiReader.OnControlChange(DeltaTime, ControllerNumber, Value);
		}
		break;
		case EMidiMessage::ProgramChange:
		{
			uint8_t ProgramNumber = Read<uint8_t>(Stream);
			MidiReader.OnProgramChange(DeltaTime, ProgramNumber);
		}
		break;
		case EMidiMessage::ChannelPressure:
		{
			uint8_t Pressure = Read<uint8_t>(Stream);
			MidiReader.OnChannelPressure(DeltaTime, Pressure);
		}
		break;
		case EMidiMessage::PitchWheelChange:
		{
			uint8_t LSB = Read<uint8_t>(Stream);
			uint8_t MSB = Read<uint8_t>(Stream);
			uint16_t Value = (MSB << 7) + LSB;
			MidiReader.OnPitchWheelChange(DeltaTime, Value);
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
		uint32_t DeltaTime = ReadVariableLength(Stream);
		uint8_t Status = Read<uint8_t>(Stream);

		if (!(Status & 0x80))
		{
			Status = ReadData.RunningStatus;
			Stream.seekg(-1, std::ios_base::cur);
		}

		if (Status == 0xFF)
		{
			return ReadMetaEvent(ReadData, Stream, DeltaTime, MidiReader);
		}
		else if (Status == 0xF0)
		{
			return ReadSysexEvent(ReadData, Stream, DeltaTime, MidiReader);
		}
		else if (Status == 0xF7)
		{
			return ReadSysexEscape(ReadData, Stream, DeltaTime, MidiReader);
		}
		
		ReadData.RunningStatus = Status;

		return ReadMidiEvent(ReadData, Stream, Status, DeltaTime, MidiReader);
	}

	bool ReadTrack(SReadData& ReadData, uint32_t Length, std::istream& Stream, IMidiReader& MidiReader)
	{
		if (Length < 1)
		{
			SetError(ReadData, "Corrupt track encountered");
			return false;
		}

		ReadData.NumReadTracks++;

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
		uint32_t Length = SwapBytes(Read<uint32_t>(Stream));

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
			return ReadHeader(ReadData, Length, Stream, MidiReader);
		}
		case 1802654797: // MTrk
		{
			return ReadTrack(ReadData, Length, Stream, MidiReader);
		}
		default:
		{
			Stream.ignore(Length);
			MidiReader.OnUnknownChunk(Type, Length);
		}
			break;
		}

		return true;
	}

	bool ReadMidi(std::istream& Stream, IMidiReader& MidiReader)
	{
		SReadData ReadData;
		try
		{
			while (ReadChunk(ReadData, Stream, MidiReader))
				;

			if (ReadData.Status == EStatus::Running)
			{
				SetError(ReadData, "Finished reading with an unhandled error");
			}
			else if (!ReadData.bReadHeader)
			{
				SetError(ReadData, "Corrupt midi file. Could not find a header");
			}
			else if (ReadData.NumReadTracks != ReadData.ExpectingNumTracks)
			{
				SetError(ReadData, "Corrupt midi file. Number of tracks read differs from header");
			}
		}
		catch (int e)
		{
			if (e == 20)
			{
				SetError(ReadData, "Corrupt midi file. Infinite loop when reading variable length");
			}
		}

		if (ReadData.Status == EStatus::Success)
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
