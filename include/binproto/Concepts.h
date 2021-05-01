#ifndef BINPROTO_CONCEPTS_H
#define BINPROTO_CONCEPTS_H

#include <binproto/BufferReader.h>

namespace binproto {

	/**
	 * This concept constrains to types that are readable via a BufferReader.
	 */
	template <class T>
	concept Readable = requires(T t, BufferReader reader) {
		{ t.Read(reader) } -> std::convertible_to<bool>;
	};

	/**
	 * This concept constrains to types that are writable to a BufferWriter.
	 */
	template <class T>
	concept Writable = requires(T t, BufferWriter writer) {
		// T::Write must take a BufferWriter& only, and can only
		// return void (nothing else)
		{ t.Write(writer) } -> std::same_as<void>;
	};

} // namespace binproto

#endif //BINPROTO_CONCEPTS_H
