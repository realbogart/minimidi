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

	enum class EMetaEvent : uint8_t
	{
		SequenceNumber = 0x00,
		TextEvent = 0x01,
		CopyrightNotice = 0x02,
		TrackName = 0x03,
		InstrumentName = 0x04,
		Lyric = 0x05,
		Marker = 0x06,
		CuePoint = 0x07,
		ChannelPrefix = 0x20,
		EndOfTrack = 0x2F,
		SetTempo = 0x51,
		SMPTEOffset = 0x54,
		TimeSignature = 0x58,
		KeySignature = 0x59,
		SequencerSpecific = 0x7F
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
		virtual void OnNoteOff(uint32_t DeltaTime, uint8_t Key, uint16_t Velocity) = 0;
		virtual void OnNoteOn(uint32_t DeltaTime, uint8_t Key, uint16_t Velocity) = 0;
		virtual void OnPolyphonicKeyPressure(uint32_t DeltaTime, uint8_t Key, uint16_t Pressure) = 0;
		virtual void OnControlChange(uint32_t DeltaTime, uint8_t ControllerNumber, uint8_t Value) = 0;
		virtual void OnProgramChange(uint32_t DeltaTime, uint8_t ProgramNumber) = 0;
		virtual void OnChannelPressure(uint32_t DeltaTime, uint8_t Pressure) = 0;
		virtual void OnPitchWheelChange(uint32_t DeltaTime, uint16_t Value) = 0;

		// Meta events
		virtual void OnEndOfTrack(uint32_t DeltaTime) = 0;
		virtual void OnSequenceNumber(uint32_t DeltaTime, uint16_t Number) = 0;
		virtual void OnTextEvent(uint32_t DeltaTime, const char* pText) = 0;
		virtual void OnCopyrightNotice(uint32_t DeltaTime, const char* pText) = 0;
		virtual void OnTrackName(uint32_t DeltaTime, const char* pTrackName) = 0;
		virtual void OnInstrumentName(uint32_t DeltaTime, const char* pInstrumentName) = 0;
		virtual void OnLyric(uint32_t DeltaTime, const char* pLyric) = 0;
		virtual void OnMarker(uint32_t DeltaTime, const char* pMarker) = 0;
		virtual void OnCuePoint(uint32_t DeltaTime, const char* pCuePoint) = 0;
		virtual void OnChannelPrefix(uint32_t DeltaTime, uint8_t Channel) = 0;
		virtual void OnSetTempo(uint32_t DeltaTime, uint32_t Tempo) = 0;
		virtual void OnSMPTEOffset(uint32_t DeltaTime, uint8_t Hour, uint8_t Minute, uint8_t Second, uint8_t Frame, uint8_t FrameFraction) = 0;
		virtual void OnTimeSignature(uint32_t DeltaTime, uint8_t Numerator, uint8_t Denominator, uint8_t MidiClocksPerMetronome, uint8_t Num32thPer24Midi) = 0;
		virtual void OnKeySignature(uint32_t DeltaTime, int8_t SharpsOrFlats, bool bMinor) = 0;
		virtual void OnSequencerSpecific(uint32_t DeltaTime, uint32_t Length, const char* pData) = 0;
		virtual void OnUnhandledMetaEvent(uint32_t DeltaTime, uint8_t Type, uint32_t Length, const char* pData) = 0;
	};

	bool ReadData(const char* pData, int DataLength, IMidiReader& MidiReader);
	bool ReadData(std::istream& Stream, IMidiReader& MidiReader);
}
