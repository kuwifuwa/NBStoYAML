#include "pch.h"
#include "NoteBlocksParser.h"
#include "ParseBytes.h"

NoteBlocksParser::NoteBlocksParser(std::ifstream& ins) {
	int tick = -1;
	char buffer[6];
	short jumpsTick;
	short jumpsLayer;

	while (true) {
		// Read next tick
		ins.read(buffer, 2);
		jumpsTick = ParseBytes::toShort(buffer);
		if (jumpsTick <= 0) {
			break;
		}
		tick += jumpsTick;
		int layer = -1;
		while (true) {
			// Read next layer
			ins.read(buffer, 2);
			jumpsLayer = ParseBytes::toShort(buffer);
			if (jumpsLayer == 0) break;
			layer += jumpsLayer;
			ins.read(buffer, 6);

			// Add new note block
			short waitTicks = (tick - jumpsTick == -1) ? jumpsTick - 1 : jumpsTick;
			jumpsTick = 0;
			noteblocks.push_back(NoteBlocksParser::NoteBlock { waitTicks, buffer[0], buffer[1], buffer[2], buffer[3], ParseBytes::toShort(buffer, 4) });
		}
	}
}
