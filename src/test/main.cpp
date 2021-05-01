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
 * A simple string container test,
 * which implements both the Writable and Readable concepts.
 *
 * If this can be read off the wire, we're doing great!
 */
struct StringContainer {
	std::string s1;
	std::string s2;

	bool Read(binproto::BufferReader& reader) {
		s1 = reader.ReadString();
		s2 = reader.ReadString();
		return true;
	}

	void Write(binproto::BufferWriter& writer) const {
		writer.WriteString(s1);
		writer.WriteString(s2);
	}
};


/**
 * A sample message payload
 */
struct MyMessage : public binproto::Message<0x1, 0xB1790001, MyMessage> {
	std::uint32_t n;

	bool Read_(binproto::BufferReader& reader) {
		n = reader.ReadUint32();
		return true;
	}

	void Write_(binproto::BufferWriter& writer) const {
		writer.WriteUint32(n);
	}
};

int main() {
	binproto::BufferWriter writer(4);

	//binproto::Write<StringContainer>(writer, { "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" });
	//binproto::Write<StringContainer>(writer, { "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" });
	//binproto::Write<StringContainer>(writer, { "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" });
	//binproto::Write<StringContainer>(writer, { "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" });
	binproto::Write<MyMessage>(writer, { .n = 32 });
	binproto::Write<MyMessage>(writer, { .n = 64 });
	binproto::Write<MyMessage>(writer, { .n = 128 });
	binproto::Write<MyMessage>(writer, { .n = 256 });

	auto buf = writer.Release();

	// Write the bytes we wrote to the buffer to stdout for fun.
	// Also lets us analyze the written data with hexdump!
	std::cout.write((char*)buf.data(), buf.size());

	binproto::BufferReader reader(buf.data(), buf.size());

	while(true) {
		auto strc = binproto::Read<MyMessage>(reader);

		if(strc.has_value()) {
			auto& read = strc.value();
			std::cerr << "N : " << read.n << '\n';
		} else {
			break;
		}
	}
}