//
// Created by lily on 5/1/2021.
//

#ifndef BINPROTO_MESSAGE_H
#define BINPROTO_MESSAGE_H

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>

namespace binproto {

	/**
	 * A message header.
	 *
	 * \tparam ID Message type code.
	 * \tparam MAGIC Message magic.
	 * \tparam Payload Inherited payload class.
	 */
	template<std::uint8_t ID, std::uint32_t MAGIC, class Payload>
	struct Message {
		std::uint32_t magic = MAGIC;
		std::uint8_t id = ID;

		bool Read(binproto::BufferReader& reader) {
			magic = reader.ReadUint32();

			if(magic != MAGIC)
				return false;

			id = reader.ReadByte();

			if(id != ID) {
				return false;
			}

			// This is kiiinda crusty.. but whatever
			try {
				if(!CRTPHelper()->Read_(reader))
					return false;
			} catch(std::exception& ex) {
				return false;
			}
			return true;
		}

		void Write(binproto::BufferWriter& writer) const {
			writer.WriteUint32(MAGIC);
			writer.WriteByte(ID);

			// call the payload write function
			CRTPHelper()->Write_(writer);
		}

	   private:

		constexpr Payload* CRTPHelper() const {
			return (Payload*)this;
		}
	};


}

#endif //BINPROTO_MESSAGE_H
