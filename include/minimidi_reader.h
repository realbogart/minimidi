#pragma once

#include "minimidi.h"

#include <ostream>

namespace MiniMidi
{
	class MidiLogReader : public IMidiReader
	{
	public:
		MidiLogReader(std::ostream& Stream);

		// Reading
		virtual void OnSuccess() override;
		virtual void OnError(const char* pErrorMessage) override;

		// Chunks
		virtual void OnTrack(uint32_t Length) override;
		virtual void OnHeader(uint16_t Format, uint16_t Tracks, uint16_t Division) override;

		// Midi messages
		virtual void OnNoteOff(uint8_t Key, uint16_t Velocity) override;
		virtual void OnNoteOn(uint8_t Key, uint16_t Velocity) override;
		virtual void OnPolyphonicKeyPressure(uint8_t Key, uint16_t Pressure) override;
		virtual void OnControlChange(uint8_t ControllerNumber, uint8_t Value) override;
		virtual void OnProgramChange(uint8_t ProgramNumber) override;
		virtual void OnChannelPressure(uint8_t Pressure) override;
		virtual void OnPitchWheelChange(uint16_t Value) override;

	private:
		std::ostream& _Stream;
		uint32_t TrackCount = 0;
	};
}
