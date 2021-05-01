//
// Various shorthand utilities
// Created by lily on 4/30/2021.
//

#ifndef BINPROTO_UTILS_H
#define BINPROTO_UTILS_H

#include <binproto/BufferReader.h>
#include <binproto/Concepts.h>
#include <optional>

namespace binproto {

	/**
	 * A helper function to read Readable objects.
	 * Automatically swallows overrun exceptions, to avoid the pain of doing that in your code.
	 * Also automatically rewinds the buffer so you can try reading again.
	 *
	 * This function only cares about Readable objects,
	 * for primitive types or buffers use the BufferReader methods directly.
	 *
	 * \tparam T Type to read
	 * \param[in] reader Reader to use
	 * \return a fully read-in T, or a null optional on any errors
	 */
	template <Readable T>
	inline std::optional<T> Read(BufferReader& reader) {
		try {
			T obj;

			if(!obj.Read(reader)) {
				reader.Rewind();
				return std::nullopt;
			}

			return obj;
		} catch(std::exception& ex) {
			// Probably a overrun
			reader.Rewind();
			return std::nullopt;
		}
	}

	/**
	 * A helper function to write Writable objects to a given BufferWriter.
	 *
	 * This function only cares about Writable objects,
	 * for primitive types or buffers use the BufferWriter methods directly.
	 *
	 * \tparam T Type to write
	 * \param[in] writer Writer to use
 	 */
	template <Writable T>
	constexpr void Write(BufferWriter& writer, const T& object) {
		object.Write(writer);
	}

} // namespace binproto

#endif //BINPROTO_UTILS_H
