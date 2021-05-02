#ifndef BINPROTO_BUFFERWRITER_H
#define BINPROTO_BUFFERWRITER_H

#include <cstdint>
#include <string>
#include <vector>

namespace binproto {

	/**
	 * Buffer writer, for writing wire format types to a buffer.
	 */
	struct BufferWriter {
		/**
		 * Constructor. Also grows to a given size immediately.
		 * \param starting_size Starting size of the buffer
		 */
		explicit BufferWriter(std::size_t starting_size);

		/**
		 * Release the written buffer to the given returned vector.
		 * Once this function is called, the internal data buffer is reset,
		 * and bytes (aka, a new message) can be writen once again.
		 */
		std::vector<std::uint8_t> Release();

		void WriteByte(std::uint8_t byte);

		void WriteUint16(std::uint16_t val);
		void WriteInt16(std::int16_t val);

		void WriteUint32(std::uint32_t val);
		void WriteInt32(std::int32_t val);

		void WriteUint64(std::uint64_t val);
		void WriteInt64(std::int64_t val);

		// TODO: WriteFloat/WriteDouble
		// probably not going to be done unless people reaaally want it?

		void WriteString(const std::string_view& string);
		void WriteBytes(const std::vector<std::uint8_t>& bytes);

	   private:
		void Grow(std::size_t grow_by);

		std::vector<std::uint8_t> buffer_;
		std::size_t cur_index_;
	};

} // namespace binproto

#endif //BINPROTO_BUFFERWRITER_H
