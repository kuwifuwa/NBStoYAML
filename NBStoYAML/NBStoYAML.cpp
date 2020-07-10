#include <iostream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "HeaderParser.h"
#include "NoteBlocksParser.h"
const char& dsep = Constants::dataSeparator;

inline void assert(bool condition, const char* message) {
	if (!condition) {
		std::cout << message;
		throw false;
	}
}

void writeOutput(std::ofstream& outs, const HeaderParser& head, const NoteBlocksParser& nb) {
	outs << Constants::yamlListName << ":\n";
	outs << "- " << +head.version << '\n';
	outs << "- \"" << +head.vanillaInstrumentCount << dsep << head.tempo << dsep <<
		head.isLooped << dsep << +head.maxLoopCount << dsep << head.loopStartTick << "\"";
	for (auto nb : nb.noteblocks) {
		outs << "\n- \"" << nb.waitTicks << dsep << +nb.instrument << dsep << +nb.key << dsep << +nb.velocity << '"';
	}
}

int main(int argc, char** argv) {
	try {
		assert(argc >= 3, Constants::usage);

		// Open input file, read and parse input
		std::ifstream inputFile { argv[1], std::ios::binary | std::ios::in };
		assert(!inputFile.fail(), Constants::inputFileUnableOpen);
		HeaderParser header { inputFile };
		NoteBlocksParser noteBlocks { inputFile };
		inputFile.close();
		
		// Open output file and write output
		std::ofstream outputFile { argv[2] };
		assert(!outputFile.fail(), Constants::outputFileUnableOpen);
		writeOutput(outputFile, header, noteBlocks);
		outputFile.close();
	}
	catch (bool) {
		return 1;
	}
	
	return 0;
}
