#include "pch.h"
#include <cinttypes>

/* Parsing functions reverses byte order: 0B 00 00 00 -> 00 00 00 0B -> 11
 * Note block studio stores numbers this way. */

namespace ParseBytes {
int32_t toInt32(char* bytes, int start = 0) {
	int32_t res = 0;
	for (int i = 0; i < 4; i++)
		res |= (static_cast<int32_t>(bytes[i + start]) & 0xFF) << i * 8;
	return res;
}

short toShort(char* bytes, int start = 0) {
	short res = 0;
	for (int i = 0; i < 2; i++)
		res |= (static_cast<int32_t>(bytes[i + start]) & 0xFF) << i * 8;
	return res;
}
}