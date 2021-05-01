//
// Simple test thing
// Created by lily on 4/29/2021.
//

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>

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

int main() {
	binproto::BufferWriter writer(4);

	binproto::Write<StringContainer>(writer, { "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" });
	binproto::Write<StringContainer>(writer, { "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" });
	binproto::Write<StringContainer>(writer, { "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" });
	binproto::Write<StringContainer>(writer, { "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" });

	auto buf = writer.Release();

	// Write the bytes we wrote to the buffer to stdout for fun.
	// Also lets us analyze the written data with hexdump!
	std::cout.write((char*)buf.data(), buf.size());

	binproto::BufferReader reader(buf.data(), buf.size());

	while(true) {
		auto strc = binproto::Read<StringContainer>(reader);

		if(strc.has_value()) {
			auto& read = strc.value();
			std::cerr << "S1: \"" << read.s1 << "\"\n";
			std::cerr << "S2: \"" << read.s2 << "\"\n";
		} else {
			break;
		}
	}
}