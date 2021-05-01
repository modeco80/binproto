#include <binproto/BufferWriter.h>
#include <binproto/EndianUtils.h>

namespace binproto {

	BufferWriter::BufferWriter(std::size_t starting_size)
		: cur_index_(0) {
		buffer_.resize(starting_size);
	}

	void BufferWriter::Grow(std::size_t grow_by) {
		buffer_.resize(buffer_.size() + grow_by);
	}

	std::vector<std::uint8_t> BufferWriter::Release() {
		std::vector<std::uint8_t> vec(buffer_.size());
		memcpy(&vec[0], &buffer_[0], buffer_.size());

		buffer_.clear();
		return vec;
	}

	void BufferWriter::WriteByte(std::uint8_t byte) {
		if((cur_index_ + sizeof(std::uint8_t)) > buffer_.size())
			Grow(sizeof(std::uint8_t));

		buffer_[cur_index_++] = byte;
	}

	void BufferWriter::WriteUint16(std::uint16_t val) {
		// Grow the buffer if we need to
		if((cur_index_ + sizeof(std::uint16_t)) > buffer_.size())
			Grow(sizeof(std::uint16_t));

		internal::WriteBE<std::uint16_t>(&buffer_[cur_index_], val);
		cur_index_ += sizeof(std::uint16_t);
	}

	void BufferWriter::WriteInt16(std::int16_t val) {
		WriteUint16(reinterpret_cast<std::uint16_t&>(val));
	}

	void BufferWriter::WriteUint32(std::uint32_t val) {
		// Grow the buffer if we need to
		if((cur_index_ + sizeof(std::uint32_t)) > buffer_.size())
			Grow(sizeof(std::uint32_t));

		internal::WriteBE<std::uint32_t>(&buffer_[cur_index_], val);
		cur_index_ += sizeof(std::uint32_t);
	}

	void BufferWriter::WriteInt32(std::int32_t val) {
		WriteUint32(reinterpret_cast<std::uint32_t&>(val));
	}

	void BufferWriter::WriteUint64(std::uint64_t val) {
		// Grow the buffer if we need to
		if((cur_index_ + sizeof(std::uint64_t)) > buffer_.size())
			Grow(sizeof(std::uint64_t));

		internal::WriteBE<std::uint64_t>(&buffer_[cur_index_], val);
		cur_index_ += sizeof(std::uint64_t);
	}

	void BufferWriter::WriteInt64(std::int64_t val) {
		WriteUint64(reinterpret_cast<std::uint64_t&>(val));
	}

	void BufferWriter::WriteString(const std::string_view& string) {
		// Write length prefix and then grow the buffer large enough to fit the string
		WriteUint32(string.length());
		Grow(string.length());

		memcpy(&buffer_[cur_index_], &string[0], string.length());
		cur_index_ += string.length();
	}

	void BufferWriter::WriteBytes(const std::vector<std::uint8_t>& bytes) {
		// Grow the buffer large enough to fit the bytes,
		// then write the length prefix and then the bytes themselves
		WriteUint32(bytes.size());
		Grow(bytes.size());

		memcpy(&buffer_[cur_index_], &bytes[0], bytes.size());
		cur_index_ += bytes.size();
	}

} // namespace binproto