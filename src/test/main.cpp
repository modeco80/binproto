//
// Simple test thing (TODO use Catch2)
// Created by lily on 4/29/2021.
//

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>

#include <binproto/Message.h>

#include <binproto/Utils.h>

#include <iostream>

/**
 * Our sample magic value
 */
constexpr static std::uint32_t magic = 0xB1794269;

/**
 * Shorthand customization point
 * (as an example of *seriously* using the Message class template)
 */
template <std::uint8_t ID, class Payload>
using MyMessageConfig = binproto::Message<ID, magic, Payload>;

/**
 * A sample message payload
 */
struct MyMessage : public MyMessageConfig<0x1, MyMessage> {
	// In here are fields that can describe the message.
	std::uint32_t n;

	// These functions technically implement
	// Readable and Writable, however Read and Write are taken up
	// by the Message<ID, MAGIC, Payload> class template.

	bool Read_(binproto::BufferReader& reader) {
		n = reader.ReadUint32();
		return true;
	}

	void Write_(binproto::BufferWriter& writer) const {
		writer.WriteUint32(n);
	}
};

/**
 * Another sample message payload.
 */
struct AnotherMessage : public MyMessageConfig<0x2, AnotherMessage> {
	std::string str;

	bool Read_(binproto::BufferReader& reader) {
		str = reader.ReadString();
		return true;
	}

	void Write_(binproto::BufferWriter& writer) const {
		writer.WriteString(str);
	}
};

int main() {
	// tune this to see how the library deals with growing the internal buffer
	// For your use-case, tune the starting grow size effectively to allow as few
	// reallocations as possible when writing a message (but be careful to also tune for low memory pressure.)
	binproto::BufferWriter writer(32);

	// Write a sample MyMessage and AnotherMessage
	// inside of the buffer.

	binproto::Write<MyMessage>(writer, { .n = 32 });
	binproto::Write<AnotherMessage>(writer, { .str = "Obama" });

	// Calling Release() on the writer results in us getting a serialized buffer.
	// It's safe for us to send this buffer over the wire, and parse it.
	auto buf = writer.Release();

	// Write the bytes we wrote to the buffer to stdout for fun.
	// Also lets us analyze the written data with "hexdump -C" !
	std::cout.write((char*)buf.data(), buf.size());

	// Now, let's test our reading code by actually making a reader pointed to that serialized buffer!
	binproto::BufferReader reader(buf.data(), buf.size());

	// Very similar to binproto::Read<> but doesn't return nullopt, instead a bool
	// if the read was successful (T::Read() returned true and no exceptions thrown)
	// false otherwise
	auto read_helper = []<binproto::Readable T>(T& a, binproto::BufferReader& reader) -> bool {
		try {
			if(!a.Read(reader))
				return false;
		} catch(std::exception& ex) {
			return false;
		}

		return true;
	};

	for(int i = 0; i < 2; ++i) {
		// Read the WireMessageHeader header first.
		binproto::WireMessageHeader header;

		if(!read_helper(header, reader))
			break;

		// If the message is a MyMessage, then parse the message as a
		// MyMessage.
		if(header.Is<MyMessage>()) {
			MyMessage message;
			message.header = header;

			if(read_helper(message, reader)) {
				// message parsed successfully
				std::cerr << "N: " << message.n << "\n";
			}
		}

		// Otherwise, if it's AnotherMessage,
		// parse it like that.
		if(header.Is<AnotherMessage>()) {
			AnotherMessage message;
			message.header = header;

			if(read_helper(message, reader)) {
				std::cerr << "AnotherMessage: STR: \"" << message.str << "\"\n";
			}
		}
	}
}