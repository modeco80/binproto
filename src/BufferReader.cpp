#include <binproto/BufferReader.h>
#include <binproto/EndianUtils.h>

#include <cstring>

namespace {

	/**
 	 * Exception emitted on buffer overrun
 	 */
	struct BufferOverrun : public std::exception {
		explicit BufferOverrun(std::size_t size)
			: size_(size) {
		}

		const char* what() const noexcept override {
			thread_local static char buf[256];
			std::size_t index {};

			memset(&buf[0], 0, sizeof(buf));

			// This is a *really* beyond ugly hack,
			// but we don't need to format the majority of this buffer except for one item
			// ... so it works well enough. Perhaps if this gets rewritten to show more info, this will be removed?

#define MEMCPY_STRING(str)                            \
	memcpy(&buf[index], (void*)str, sizeof(str) - 1); \
	index += sizeof(str) - 1

			MEMCPY_STRING("Attempted to overrun the buffer trying to read ");
			index += sprintf(&buf[index], "%llu", size_);
			MEMCPY_STRING(" bytes of data");

#undef MEMCPY_STRING

			return &buf[0];
		}

	   private:
		/**
		 * Stored size we tried to read.
		 */
		std::size_t size_;
	};

} // namespace

namespace binproto {

	BufferReader::BufferReader(const std::uint8_t* buffer, std::size_t buffer_size)
		: begin(buffer),
		  end(buffer + buffer_size),
		  cur(buffer) {
	}

	void BufferReader::BoundsCheck(std::size_t size) const {
		if((cur + size) > end)
			throw BufferOverrun(size);
	}

	std::uint8_t BufferReader::ReadByte() {
		BoundsCheck(sizeof(std::uint8_t));
		return *cur++;
	}

	std::uint16_t BufferReader::ReadUint16() {
		BoundsCheck(sizeof(std::uint16_t));

		auto val = internal::ReadBE<std::uint16_t>(cur);
		cur += sizeof(std::uint16_t);

		return val;
	}

	std::int16_t BufferReader::ReadInt16() {
		auto v = ReadUint16();
		return reinterpret_cast<std::int16_t&>(v);
	}

	std::uint32_t BufferReader::ReadUint32() {
		BoundsCheck(sizeof(std::uint32_t));

		auto val = internal::ReadBE<std::uint32_t>(cur);
		cur += sizeof(std::uint32_t);

		return val;
	}

	std::int32_t BufferReader::ReadInt32() {
		auto v = ReadUint32();
		return reinterpret_cast<std::int32_t&>(v);
	}

	std::uint64_t BufferReader::ReadUint64() {
		BoundsCheck(sizeof(std::uint64_t));

		auto val = internal::ReadBE<std::uint64_t>(cur);
		cur += sizeof(std::uint64_t);

		return val;
	}

	std::int64_t BufferReader::ReadInt64() {
		auto v = ReadUint64();
		return reinterpret_cast<std::int64_t&>(v);
	}

	BufferReader::LengthType BufferReader::ReadLength() {
		auto length = ReadUint32();
		// Check if we can actually read that many bytes from the buffer here too
		BoundsCheck((std::size_t)length * sizeof(std::uint8_t));
		return length;
	}

	// Untested

	/*
	float BufferReader::ReadFloat() {
		auto v = ReadUint32();
		return (float)v;
	}

	double BufferReader::ReadDouble() {
		auto v = ReadUint64();
		return (double)v;
	}
	 */

	std::string BufferReader::ReadString() {
		// We internally serialize strings ala
		// struct {
		//  BufferReader::LengthType length;
		//  char str[length]; // NO \0!!!!!!!!!!
		// };
		auto len = ReadLength();

		std::string str;
		str.resize(len);

		//for(BufferReader::LengthType i = 0; i < len; ++i)
		//	str[i] = ReadByte();
		memcpy(&str[0], cur, len);
		cur += len;

		return str;
	}

	std::vector<std::uint8_t> BufferReader::ReadBytes() {
		// We store byte[] the same way as above.
		auto len = ReadLength();
		std::vector<std::uint8_t> vector;

		vector.resize(len);

		memcpy(&vector[0], cur, len);
		cur += len;

		return vector;
	}

} // namespace binproto