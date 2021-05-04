#ifndef BINPROTO_VERSION_H
#define BINPROTO_VERSION_H

#include <cstdint>
#include <string>

namespace binproto::version {

	/**
	 * Major version of BinProto library
	 */
	constexpr std::uint8_t major = 1;

	/**
	 * Minor version of BinProto library
	 */
	constexpr std::uint8_t minor = 0;

	/**
	 * Retrieve a string of the current library version.
	 * This is only initialized the first time it's called;
	 * all other times simply return a reference to the initialized value
	 */
	const std::string& String();
} // namespace binproto::version

#endif //BINPROTO_VERSION_H
