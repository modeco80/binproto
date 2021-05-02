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

template<std::uint8_t ID, class Payload>
using MyMessageConfig = binproto::Message<ID, 0xB1790001, Payload>;

/**
 * A sample message payload
 */
struct MyMessage : public /*binproto::Message<0x1, 0xB1790001, MyMessage>*/ MyMessageConfig<0x1, MyMessage> {
	std::uint32_t n;

	bool Read_(binproto::BufferReader& reader) {
		n = reader.ReadUint32();
		return true;
	}

	void Write_(binproto::BufferWriter& writer) const {
		writer.WriteUint32(n);
	}
};

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
	binproto::BufferWriter writer(4);

	//binproto::Write<StringContainer>(writer, { "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb" });
	binproto::Write<MyMessage>(writer, { .n = 32 });
	binproto::Write<AnotherMessage>(writer, { .str = "Obama" });


	auto buf = writer.Release();

	// Write the bytes we wrote to the buffer to stdout for fun.
	// Also lets us analyze the written data with hexdump!
	std::cout.write((char*)buf.data(), buf.size());

	binproto::BufferReader reader(buf.data(), buf.size());

	// very similar to binproto::Read<> but doesn't return nullopt, instead a bool
	auto read_helper = []<binproto::Readable T>(T& a, binproto::BufferReader& reader) {
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


		// If the message is a MyMessage, then process it!
		if(header.Is<MyMessage>()) {
			MyMessage message;
			message.header = header;

			if(read_helper(message, reader)) {
				// message parsed successfully
				std::cerr << "N: " << message.n << "\n";
			}
		}

		if(header.Is<AnotherMessage>()) {
			AnotherMessage message;
			message.header = header;

			if(read_helper(message, reader)) {
				std::cerr << "AnotherMessage: STR: \"" << message.str << "\"\n";
			}
		}


	}


	/*
	while(true) {
		auto strc = binproto::Read<MyMessage>(reader);

		if(strc.has_value()) {
			auto& read = strc.value();
			std::cerr << "N : " << read.n << '\n';
		} else {
			break;
		}
	}
	 */
}