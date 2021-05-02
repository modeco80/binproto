#ifndef BINPROTO_ENDIANUTILS_H
#define BINPROTO_ENDIANUTILS_H

#include <cstdint>
#include <climits>

// TODO: Even though this is internal and limited specialization-wize,
// maybe emit these in the main binproto namespace?

namespace binproto::internal {

	template <class T>
	T ReadBE(const std::uint8_t* base);

	template <class T>
	void WriteBE(std::uint8_t* base, const T& val);
} // namespace binproto::internal

#endif //BINPROTO_ENDIANUTILS_H
