#include "minimidi_reader.h"

#include <iostream>
#include <iomanip>

using namespace std;

namespace MiniMidi
{
	void MidiLogReader::OnHeader(uint16_t Format, uint16_t Tracks, uint16_t Division)
	{
		_Stream << "[Header]: Format = " << Format << ", Tracks = " << Tracks << ", Division = " << Division << endl;
	}

	void MidiLogReader::OnSuccess()
	{
		_Stream << "Successfully read midi file!" << endl;
	}

	void MidiLogReader::OnError(const char* pErrorMessage)
	{
		_Stream << "[Error]: " << pErrorMessage << endl;
	}

	void MidiLogReader::OnNoteOff(uint32_t DeltaTime, uint8_t Key, uint16_t Velocity)
	{
		_Stream << "[Midi Message][dt="<< DeltaTime <<"][NoteOff]: "<<"Key = "<< (int)Key <<", Velocity = " << (int)Velocity << endl;
	}

	void MidiLogReader::OnNoteOn(uint32_t DeltaTime, uint8_t Key, uint16_t Velocity)
	{
		_Stream << "[Midi Message][dt=" << DeltaTime << "][NoteOn]: " << "Key = " << (int)Key << ", Velocity = " << (int)Velocity << endl;
	}

	void MidiLogReader::OnPolyphonicKeyPressure(uint32_t DeltaTime, uint8_t Key, uint16_t Pressure)
	{
		_Stream << "[Midi Message][dt=" << DeltaTime << "][PolyphonicKeyPressure]: " << "Key = " << (int)Key << ", Pressure = " << (int)Pressure << endl;
	}

	void MidiLogReader::OnControlChange(uint32_t DeltaTime, uint8_t ControllerNumber, uint8_t Value)
	{
		_Stream << "[Midi Message][dt=" << DeltaTime << "][ControlChange]: " << "ControllerNumber = " << (int)ControllerNumber << ", Value = " << (int)Value << endl;
	}

	void MidiLogReader::OnProgramChange(uint32_t DeltaTime, uint8_t ProgramNumber)
	{
		_Stream << "[Midi Message][dt=" << DeltaTime << "][ProgramChange]: " << "ProgramNumber = " << (int)ProgramNumber << endl;
	}

	void MidiLogReader::OnChannelPressure(uint32_t DeltaTime, uint8_t Pressure)
	{
		_Stream << "[Midi Message][dt=" << DeltaTime << "][ChannelPressure]: " << "Pressure = " << (int)Pressure << endl;
	}

	void MidiLogReader::OnPitchWheelChange(uint32_t DeltaTime, uint16_t Value)
	{
		_Stream << "[Midi Message][dt=" << DeltaTime << "][PitchWheelChange]: " << "Value = " << (int)Value << endl;
	}

	void MidiLogReader::OnEndOfTrack(uint32_t DeltaTime)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][EndOfTrack]" << endl;
	}

	void MidiLogReader::OnSequenceNumber(uint32_t DeltaTime, uint16_t Number)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][SequenceNumber]: " << (int)Number << endl;
	}

	void MidiLogReader::OnTextEvent(uint32_t DeltaTime, const char* pText)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][TextEvent]: " << pText << endl;
	}

	void MidiLogReader::OnCopyrightNotice(uint32_t DeltaTime, const char* pText)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][CopyrightNotice]: " << pText << endl;
	}

	void MidiLogReader::OnTrackName(uint32_t DeltaTime, const char* pTrackName)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][TrackName]: " << pTrackName << endl;
	}

	void MidiLogReader::OnInstrumentName(uint32_t DeltaTime, const char* pInstrumentName)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][InstrumentName]: " << pInstrumentName << endl;
	}

	void MidiLogReader::OnLyric(uint32_t DeltaTime, const char* pLyric)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][Lyric]: " << pLyric << endl;
	}

	void MidiLogReader::OnMarker(uint32_t DeltaTime, const char* pMarker)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][Marker]: " << pMarker << endl;
	}

	void MidiLogReader::OnCuePoint(uint32_t DeltaTime, const char* pCuePoint)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][CuePoint]: " << pCuePoint << endl;
	}

	void MidiLogReader::OnChannelPrefix(uint32_t DeltaTime, uint8_t Channel)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][ChannelPrefix]: " << (int)Channel << endl;
	}

	void MidiLogReader::OnSetTempo(uint32_t DeltaTime, uint32_t Tempo)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][SetTempo]: " << (int)Tempo << endl;
	}

	void MidiLogReader::OnSMPTEOffset(uint32_t DeltaTime, uint8_t Hour, uint8_t Minute, uint8_t Second, uint8_t Frame, uint8_t FrameFraction)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][SMPTEOffset]: Hour = " << (int)Hour << ", Minute = " << (int)Minute << ", Second = " << (int)Minute << ", Frame = " << (int)Frame << ", FrameFraction = " << (int)FrameFraction << endl;
	}

	void MidiLogReader::OnTimeSignature(uint32_t DeltaTime, uint8_t Numerator, uint8_t Denominator, uint8_t MidiClocksPerMetronome, uint8_t Num32thPer24Midi)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][TimeSignature]: Numerator = " << (int)Numerator << ", Denominator = " << (int)Denominator << ", MidiClocksPerMetronome = " << (int)MidiClocksPerMetronome << ", Num32thPer24Midi = " << (int)Num32thPer24Midi << endl;
	}

	void MidiLogReader::OnKeySignature(uint32_t DeltaTime, int8_t SharpsOrFlats, bool bMinor)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][KeySignature]: SharpsOrFlats = " << (int)SharpsOrFlats << ", Minor = " << bMinor << endl;
	}

	void MidiLogReader::OnSequencerSpecific(uint32_t DeltaTime, uint32_t Length, const char* pData)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][SequencerSpecific]: Length = " << Length << endl;
	}

	void MidiLogReader::OnUnhandledMetaEvent(uint32_t DeltaTime, uint8_t Type, uint32_t Length, const char* pData)
	{
		_Stream << "[Meta Event][dt=" << DeltaTime << "][UnhandledMetaEvent]: Type = "<< hex << setfill('0') << setw(2)<<Type<<", Length = " << Length << endl;
	}

	void MidiLogReader::OnSysexEvent(uint32_t DeltaTime, uint32_t Length, const char* pData)
	{
		_Stream << "[Sysex Event][dt=" << DeltaTime << "]: Length = " << Length << endl;
	}

	void MidiLogReader::OnSysexEscape(uint32_t DeltaTime, uint32_t Length, const char* pData)
	{
		_Stream << "[Sysex Escape][dt=" << DeltaTime << "]: Length = " << Length << endl;
	}

	MidiLogReader::MidiLogReader(std::ostream& Stream)
		: _Stream(Stream)
	{
	}

	void MidiLogReader::OnTrack(uint32_t Length)
	{
		_Stream << "[Track "<<TrackCount<<"]: Length = " << Length << " bytes" << endl;
		TrackCount++;
	}
}
