#pragma once
namespace Constants
{
const char* usage = R"(Usage: NBStoYAML <input> <output>
   input - Required, NBS file directory
   output - Required, YAML file directory

Spaces are not allowed in file names
Supported NBS Versions: Open NBS 4
Eg. NBStoYAML MySong.nbs MySong.yml
)";

const char* inputFileUnableOpen = "Unable to open input file\n";
const char* outputFileUnableOpen = "Unable to open output file\n";

const char* yamlListName = "NoteBlockSongData";
const char dataSeparator = ',';
}