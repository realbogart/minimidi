#include "..\include\minimidi_reader.h"
#include "minimidi_reader.h"

#include <iostream>
#include <iomanip>

using namespace std;

namespace
{
	void PrintHex(std::ostream& Stream, uint32_t Length, const char* pData)
	{
		for (int i = 0; i < Length; i++)
		{
			Stream << hex << setfill('0') << setw(2) << (int)pData[i] << " ";
		}
	}
}

namespace MiniMidi
{
	void MidiLogReader::OnHeader(uint16_t Format, uint16_t Tracks, uint16_t Division)
	{
		_Stream << "[Header]: Format = " << Format << ", Tracks = " << Tracks << ", Division = " << Division << endl;
	}

	void MidiLogReader::OnUnknownChunk(uint32_t Type, uint32_t Length)
	{
		_Stream << "[UnknownChunk]: Type = " << Type << ", Length = " << Length << endl;
	}

	void MidiLogReader::OnSuccess()
	{
		_Stream << endl;
		_Stream << "[Success]" << endl;
		_Stream << "--------------------------" << endl;
		_Stream << "Tracks\t\t\t\t"<< _TrackCount << endl;
		_Stream << "Midi Messages\t\t"<< _MidiMessagesCount << endl;
		_Stream << "Meta Events\t\t\t"<< _MetaEventsCount << endl;
		_Stream << "Sysex Events\t\t"<< _SysexCount << endl;
		_Stream << "Unhandled\t\t\t"<< _UnhandledCount << endl;
		_Stream << "--------------------------" << endl;
	}

	void MidiLogReader::OnError(const char* pErrorMessage)
	{
		_Stream << "[Error]: " << pErrorMessage << endl;
		_ErrorMessage = pErrorMessage;
	}

	void MidiLogReader::OnEventStart(uint8_t Status, uint32_t DeltaTime)
	{
	}

	void MidiLogReader::OnNoteOff(uint32_t DeltaTime, uint8_t Key, uint16_t Velocity)
	{
		_Stream << "[Midi Message]["<< _MidiMessagesCount <<"][dt="<< DeltaTime <<"][NoteOff]: "<<"Key = "<< (int)Key <<", Velocity = " << (int)Velocity << endl;
		_MidiMessagesCount++;
	}

	void MidiLogReader::OnNoteOn(uint32_t DeltaTime, uint8_t Key, uint16_t Velocity)
	{
		_Stream << "[Midi Message][" << _MidiMessagesCount << "][dt=" << DeltaTime << "][NoteOn]: " << "Key = " << (int)Key << ", Velocity = " << (int)Velocity << endl;
		_MidiMessagesCount++;
	}

	void MidiLogReader::OnPolyphonicKeyPressure(uint32_t DeltaTime, uint8_t Key, uint16_t Pressure)
	{
		_Stream << "[Midi Message][" << _MidiMessagesCount << "][dt=" << DeltaTime << "][PolyphonicKeyPressure]: " << "Key = " << (int)Key << ", Pressure = " << (int)Pressure << endl;
		_MidiMessagesCount++;
	}

	void MidiLogReader::OnControlChange(uint32_t DeltaTime, uint8_t ControllerNumber, uint8_t Value)
	{
		_Stream << "[Midi Message][" << _MidiMessagesCount << "][dt=" << DeltaTime << "][ControlChange]: " << "ControllerNumber = " << (int)ControllerNumber << ", Value = " << (int)Value << endl;
		_MidiMessagesCount++;
	}

	void MidiLogReader::OnProgramChange(uint32_t DeltaTime, uint8_t ProgramNumber)
	{
		_Stream << "[Midi Message][" << _MidiMessagesCount << "][dt=" << DeltaTime << "][ProgramChange]: " << "ProgramNumber = " << (int)ProgramNumber << endl;
		_MidiMessagesCount++;
	}

	void MidiLogReader::OnChannelPressure(uint32_t DeltaTime, uint8_t Pressure)
	{
		_Stream << "[Midi Message][" << _MidiMessagesCount << "][dt=" << DeltaTime << "][ChannelPressure]: " << "Pressure = " << (int)Pressure << endl;
		_MidiMessagesCount++;
	}

	void MidiLogReader::OnPitchWheelChange(uint32_t DeltaTime, uint16_t Value)
	{
		_Stream << "[Midi Message][" << _MidiMessagesCount << "][dt=" << DeltaTime << "][PitchWheelChange]: " << "Value = " << (int)Value << endl;
		_MidiMessagesCount++;
	}

	void MidiLogReader::OnEndOfTrack(uint32_t DeltaTime)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][dt=" << DeltaTime << "][EndOfTrack]" << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnSequenceNumber(uint32_t DeltaTime, uint16_t Number)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][SequenceNumber]: " << (int)Number << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnTextEvent(uint32_t DeltaTime, const char* pText)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][TextEvent]: " << pText << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnCopyrightNotice(uint32_t DeltaTime, const char* pText)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][CopyrightNotice]: " << pText << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnTrackName(uint32_t DeltaTime, const char* pTrackName)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][TrackName]: " << pTrackName << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnInstrumentName(uint32_t DeltaTime, const char* pInstrumentName)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][InstrumentName]: " << pInstrumentName << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnLyric(uint32_t DeltaTime, const char* pLyric)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][Lyric]: " << pLyric << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnMarker(uint32_t DeltaTime, const char* pMarker)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][Marker]: " << pMarker << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnCuePoint(uint32_t DeltaTime, const char* pCuePoint)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][CuePoint]: " << pCuePoint << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnChannelPrefix(uint32_t DeltaTime, uint8_t Channel)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][ChannelPrefix]: " << (int)Channel << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnMidiPort(uint32_t DeltaTime, uint8_t Port)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][MidiPort]: " << (int)Port << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnSetTempo(uint32_t DeltaTime, uint32_t Tempo)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][SetTempo]: " << (int)Tempo << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnSMPTEOffset(uint32_t DeltaTime, uint8_t Hour, uint8_t Minute, uint8_t Second, uint8_t Frame, uint8_t FrameFraction)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][SMPTEOffset]: Hour = " << (int)Hour << ", Minute = " << (int)Minute << ", Second = " << (int)Minute << ", Frame = " << (int)Frame << ", FrameFraction = " << (int)FrameFraction << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnTimeSignature(uint32_t DeltaTime, uint8_t Numerator, uint8_t Denominator, uint8_t MidiClocksPerMetronome, uint8_t Num32thPer24Midi)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][TimeSignature]: Numerator = " << (int)Numerator << ", Denominator = " << (int)Denominator << ", MidiClocksPerMetronome = " << (int)MidiClocksPerMetronome << ", Num32thPer24Midi = " << (int)Num32thPer24Midi << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnKeySignature(uint32_t DeltaTime, int8_t SharpsOrFlats, bool bMinor)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][KeySignature]: SharpsOrFlats = " << (int)SharpsOrFlats << ", Minor = " << bMinor << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnSequencerSpecific(uint32_t DeltaTime, uint32_t Length, const char* pData)
	{
		_Stream << "[Meta Event]["<< _MetaEventsCount <<"][SequencerSpecific]: Length = " << Length << endl;
		_MetaEventsCount++;
	}

	void MidiLogReader::OnUnhandledMetaEvent(uint32_t DeltaTime, uint8_t Type, uint32_t Length, const char* pData)
	{
		_Stream << "[Meta Event]["<< _UnhandledCount <<"][UnhandledMetaEvent]: Type = "<< hex << setfill('0') << setw(2)<<(int)Type<<", Length = " << Length << ", Data = ";
		PrintHex(_Stream, Length, pData);
		_Stream << endl;
		_UnhandledCount++;
	}

	void MidiLogReader::OnSysexEvent(uint32_t DeltaTime, uint32_t Length, const char* pData)
	{
		_Stream << "[Sysex Event]["<< _SysexCount <<"][dt=" << DeltaTime << "]: Length = " << Length << ", Data = ";
		PrintHex(_Stream, Length, pData);
		_Stream << endl;

		_SysexCount++;
	}

	void MidiLogReader::OnSysexEscape(uint32_t DeltaTime, uint32_t Length, const char* pData)
	{
		_Stream << "[Sysex Escape][" << _SysexCount << "][dt=" << DeltaTime << "]: Length = " << Length << ", Data = ";
		PrintHex(_Stream, Length, pData);
		_Stream << endl;

		_SysexCount++;
	}

	MidiLogReader::MidiLogReader(std::ostream& Stream)
		: _Stream(Stream)
	{
	}

	void MidiLogReader::OnTrack(uint32_t Length)
	{
		_Stream << "[Track "<<_TrackCount<<"]: Length = " << Length << " bytes" << endl;
		_TrackCount++;
	}
}
