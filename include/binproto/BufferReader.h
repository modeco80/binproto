//
// Created by lily on 4/28/2021.
//

#ifndef BINPROTO_BUFFERREADER_H
#define BINPROTO_BUFFERREADER_H

#include <cstdint>
#include <string>
#include <vector>

namespace binproto {

	/**
	 * A buffer reader.
	 */
	struct BufferReader {
		/**
		 * Type used for length storage.
		 */
		using LengthType = std::uint32_t;

		BufferReader(const std::uint8_t* buffer, std::size_t buffer_size);

		// FIXME: Document the API, since it's kinda public

		// TODO: provide a Init routine to load in a buffer, so the same Reader and Writer objects can be used to read lots of buffers in

		/**
		 * Completely rewind the buffer back to the start.
		 */
		void Rewind();

		std::uint8_t ReadByte();

		std::uint16_t ReadUint16();
		std::int16_t ReadInt16();

		std::uint32_t ReadUint32();
		std::int32_t ReadInt32();

		std::uint64_t ReadUint64();
		std::int64_t ReadInt64();

		/**
		 * Read length, while bounds checking that value from our buffer
		 */
		LengthType ReadLength();

		//float ReadFloat();
		//double ReadDouble();

		std::string ReadString();
		std::vector<std::uint8_t> ReadBytes();

	   private:
		/**
		 * Internal bounds checking helper
		 * Throws an internally defined exception if something is awry
		 */
		void BoundsCheck(std::size_t size) const;

		const std::uint8_t* cur;
		const std::uint8_t* begin;
		const std::uint8_t* end;
	};

} // namespace binproto

#endif //BINPROTO_BUFFERREADER_H
