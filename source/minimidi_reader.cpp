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

	MidiLogReader::MidiLogReader(std::ostream& Stream)
		: _Stream(Stream)
	{
	}

	void MidiLogReader::OnTrack(uint32_t Length)
	{
		TrackCount++;
		_Stream << "Reading track " << TrackCount << ": Length = " << Length << " bytes" << endl;
	}
}