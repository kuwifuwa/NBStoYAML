#pragma once
#include <fstream>
#include <vector>

class NoteBlocksParser
{
private:
	class NoteBlock {
	public:
		NoteBlock(short waitTicks_, char instrument_, char key_, char velocity_, char panning_, short pitch_)
			: waitTicks { waitTicks_ }, instrument { instrument_ }, key { key_ }, velocity { velocity_ }, panning { panning_ }, pitch { pitch_ }
		{}

		short waitTicks;
		char instrument;
		char key;
		char velocity;
		char panning;
		short pitch;
	};
public:
	std::vector<NoteBlock> noteblocks;
	NoteBlocksParser(std::ifstream& ins);
};

