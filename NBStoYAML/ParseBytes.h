#pragma once
#include <cinttypes>

namespace ParseBytes {

/* Parsing functions reverses byte order: 0B 00 00 00 -> 00 00 00 0B -> 11
 * Note block studio stores numbers this way. */

int32_t toInt32(char* bytes, int start = 0);
short toShort(char* bytes, int start = 0);
}