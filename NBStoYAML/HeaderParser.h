#pragma once
#include <fstream>

class HeaderParser
{
public:
	char version;

	char vanillaInstrumentCount;
	short length;
	short layerCount;
	short tempo;

	bool isLooped;
	char maxLoopCount;
	short loopStartTick;

	HeaderParser(std::ifstream& ins);
};

