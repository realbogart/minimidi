#include "..\include\minimidi_reader.h"
#include "..\include\minimidi_reader.h"
#include "..\include\minimidi_reader.h"
#include "..\include\minimidi_reader.h"
#include "..\include\minimidi_reader.h"
#include "minimidi_reader.h"

#include <iostream>

using namespace std;

namespace MiniMidi
{
	void MidiLogReader::OnHeader(uint16_t Format, uint16_t Tracks, uint16_t Division)
	{
		_Stream << "Reading header: Format = " << Format << ", Tracks = " << Tracks << ", Division = " << Division << endl;
	}

	void MidiLogReader::OnSuccess()
	{
		_Stream << "Successfully read midi file!" << endl;
	}

	void MidiLogReader::OnError(const char* pErrorMessage)
	{
		_Stream << "An error occurred: " << pErrorMessage << endl;
	}

	void MidiLogReader::OnNoteOff(uint8_t Key, uint16_t Velocity)
	{
		_Stream << "[Midi message][NoteOff]: "<<"Key = "<< (int)Key <<", Velocity = " << (int)Velocity << endl;
	}

	void MidiLogReader::OnNoteOn(uint8_t Key, uint16_t Velocity)
	{
		_Stream << "[Midi message][NoteOn]: " << "Key = " << (int)Key << ", Velocity = " << (int)Velocity << endl;
	}

	void MidiLogReader::OnPolyphonicKeyPressure(uint8_t Key, uint16_t Pressure)
	{
		_Stream << "[Midi message][PolyphonicKeyPressure]: " << "Key = " << (int)Key << ", Pressure = " << (int)Pressure << endl;
	}

	void MidiLogReader::OnControlChange(uint8_t ControllerNumber, uint8_t Value)
	{
		_Stream << "[Midi message][ControlChange]: " << "ControllerNumber = " << (int)ControllerNumber << ", Value = " << (int)Value << endl;
	}

	void MidiLogReader::OnProgramChange(uint8_t ProgramNumber)
	{
		_Stream << "[Midi message][ProgramChange]: " << "ProgramNumber = " << (int)ProgramNumber << endl;
	}

	void MidiLogReader::OnChannelPressure(uint8_t Pressure)
	{
		_Stream << "[Midi message][ChannelPressure]: " << "Pressure = " << (int)Pressure << endl;
	}

	void MidiLogReader::OnPitchWheelChange(uint16_t Value)
	{
		_Stream << "[Midi message][PitchWheelChange]: " << "Value = " << (int)Value << endl;
	}

	MidiLogReader::MidiLogReader(std::ostream& Stream)
		: _Stream(Stream)
	{
	}

	void MidiLogReader::OnTrack(uint32_t Length)
	{
		_Stream << "Reading track " << TrackCount << ": Length = " << Length << " bytes" << endl;
		TrackCount++;
	}
}
