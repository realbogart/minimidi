#pragma once

#include <stdint.h>
#include <istream>

namespace MiniMidi
{
	class IMidiReader
	{
	public:
		virtual void OnHeader(uint16_t Format, uint16_t Tracks, uint16_t Division) = 0;
		virtual void OnTrack(uint32_t Length) = 0;
		virtual void OnSuccess() = 0;
		virtual void OnError(const char* pErrorMessage) = 0;
	};

	bool ReadData(const char* pData, int DataLength, IMidiReader& MidiReader);
	bool ReadData(std::istream& Stream, IMidiReader& MidiReader);
}
