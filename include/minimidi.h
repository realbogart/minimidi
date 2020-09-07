#pragma once

#include <stdint.h>
#include <istream>

namespace MiniMidi
{
	enum class EMidiMessage : uint8_t
	{
		NoteOff = 0x80,
		NoteOn = 0x90,
		PolyphonicKeyPressure = 0xA0,
		ControlChange = 0xB0,
		ProgramChange = 0xC0,
		ChannelPressure = 0xD0,
		PitchWheelChange = 0xE0
	};

	class IMidiReader
	{
	public:
		// Reading
		virtual void OnSuccess() = 0;
		virtual void OnError(const char* pErrorMessage) = 0;

		// Chunks
		virtual void OnHeader(uint16_t Format, uint16_t Tracks, uint16_t Division) = 0;
		virtual void OnTrack(uint32_t Length) = 0;

		// Midi messages
		virtual void OnNoteOff(uint8_t Key, uint16_t Velocity) = 0;
		virtual void OnNoteOn(uint8_t Key, uint16_t Velocity) = 0;
		virtual void OnPolyphonicKeyPressure(uint8_t Key, uint16_t Pressure) = 0;
		virtual void OnControlChange(uint8_t ControllerNumber, uint8_t Value) = 0;
		virtual void OnProgramChange(uint8_t ProgramNumber) = 0;
		virtual void OnChannelPressure(uint8_t Pressure) = 0;
		virtual void OnPitchWheelChange(uint16_t Value) = 0;
	};

	bool ReadData(const char* pData, int DataLength, IMidiReader& MidiReader);
	bool ReadData(std::istream& Stream, IMidiReader& MidiReader);
}
