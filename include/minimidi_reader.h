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
		virtual void OnNoteOff(uint32_t DeltaTime, uint8_t Key, uint16_t Velocity) override;
		virtual void OnNoteOn(uint32_t DeltaTime, uint8_t Key, uint16_t Velocity) override;
		virtual void OnPolyphonicKeyPressure(uint32_t DeltaTime, uint8_t Key, uint16_t Pressure) override;
		virtual void OnControlChange(uint32_t DeltaTime, uint8_t ControllerNumber, uint8_t Value) override;
		virtual void OnProgramChange(uint32_t DeltaTime, uint8_t ProgramNumber) override;
		virtual void OnChannelPressure(uint32_t DeltaTime, uint8_t Pressure) override;
		virtual void OnPitchWheelChange(uint32_t DeltaTime, uint16_t Value) override;

		// Meta events
		virtual void OnEndOfTrack(uint32_t DeltaTime) override;
		virtual void OnSequenceNumber(uint32_t DeltaTime, uint16_t Number) override;
		virtual void OnTextEvent(uint32_t DeltaTime, const char* pText) override;
		virtual void OnCopyrightNotice(uint32_t DeltaTime, const char* pText) override;
		virtual void OnTrackName(uint32_t DeltaTime, const char* pTrackName) override;
		virtual void OnInstrumentName(uint32_t DeltaTime, const char* pInstrumentName) override;
		virtual void OnLyric(uint32_t DeltaTime, const char* pLyric) override;
		virtual void OnMarker(uint32_t DeltaTime, const char* pMarker) override;
		virtual void OnCuePoint(uint32_t DeltaTime, const char* pCuePoint) override;
		virtual void OnChannelPrefix(uint32_t DeltaTime, uint8_t Channel) override;
		virtual void OnSetTempo(uint32_t DeltaTime, uint32_t Tempo) override;
		virtual void OnSMPTEOffset(uint32_t DeltaTime, uint8_t Hour, uint8_t Minute, uint8_t Second, uint8_t Frame, uint8_t FrameFraction) override;
		virtual void OnTimeSignature(uint32_t DeltaTime, uint8_t Numerator, uint8_t Denominator, uint8_t MidiClocksPerMetronome, uint8_t Num32thPer24Midi) override;
		virtual void OnKeySignature(uint32_t DeltaTime, int8_t SharpsOrFlats, bool bMinor) override;
		virtual void OnSequencerSpecific(uint32_t DeltaTime, uint32_t Length, const char* pData) override;
		virtual void OnUnhandledMetaEvent(uint32_t DeltaTime, uint8_t Type, uint32_t Length, const char* pData) override;

	private:
		std::ostream& _Stream;
		uint32_t TrackCount = 0;
	};
}
