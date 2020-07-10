#include "pch.h"
#include "HeaderParser.h"
#include "ParseBytes.h"
#include <cassert>

HeaderParser::HeaderParser(std::ifstream& ins) {
	char buffer[4];

	// Check validity
	ins.read(buffer, 3);
	assert(buffer[0] == 0 && buffer[1] == 0 && "First 2 bytes must be 0");
	assert(buffer[2] == 4 && "NBS version not supported");
	this->version = buffer[2];

	// Read instrument count, length, layer count
	ins.read(buffer, 1);
	this->vanillaInstrumentCount = buffer[0];
	ins.read(buffer, 4);
	this->length = ParseBytes::toShort(buffer, 0);
	this->layerCount = ParseBytes::toShort(buffer, 2);

	// Ignore song name, author, original author, description (4 strings)
	for (int i = 0; i < 4; i++) {
		ins.read(buffer, 4);
		ins.seekg(ParseBytes::toInt32(buffer), std::ios::cur);
	}

	// Read tempo
	ins.read(buffer, 2);
	this->tempo = ParseBytes::toShort(buffer, 0);

	/* Ignore auto-saving, auto-saving duration, time signature, minutes spent, left clicks,
	 * right clicks, note blocks added, note blocks removed (3 bytes, 5 integers) */
	ins.seekg(1 * 3 + sizeof(int32_t) * 5, std::ios::cur);

	// Ignore MIDI/schematic file name (1 string)
	ins.read(buffer, 4);
	ins.seekg(ParseBytes::toInt32(buffer), std::ios::cur);

	// Read loop info
	ins.read(buffer, 4);
	this->isLooped = buffer[0];
	this->maxLoopCount = buffer[1];
	this->loopStartTick = ParseBytes::toShort(buffer, 2);
}
