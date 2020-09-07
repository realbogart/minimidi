#pragma once

#include "minimidi.h"

#include <ostream>

namespace MiniMidi
{
	class MidiLogReader : public IMidiReader
	{
	public:
		MidiLogReader(std::ostream& Stream);

		virtual void OnTrack(uint32_t Length) override;
		virtual void OnHeader(uint16_t Format, uint16_t Tracks, uint16_t Division) override;
		virtual void OnSuccess() override;
		virtual void OnError(const char* pErrorMessage) override;

	private:
		std::ostream& _Stream;
		uint32_t TrackCount = 0;
	};
}
