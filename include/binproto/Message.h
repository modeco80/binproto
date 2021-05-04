//
// Created by lily on 5/1/2021.
//

#ifndef BINPROTO_MESSAGE_H
#define BINPROTO_MESSAGE_H

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>

namespace binproto {

	/**
	 * Message wire header.
	 * This does not change across message types.
	 */
	struct WireMessageHeader {
		std::uint32_t magic;
		std::uint8_t id;

		bool read = false;

		bool Read(binproto::BufferReader& reader) {
			magic = reader.ReadUint32();
			id = reader.ReadByte();

			read = true;
			return true;
		}

		void Write(binproto::BufferWriter& writer) const {
			writer.WriteUint32(magic);
			writer.WriteByte(id);
		}

		/**
		 * Check if another fully-defined message type matches this header.
		 *
		 * \tparam Message The fully-defined message type to check.
		 */
		template <class Message>
		[[nodiscard]] bool Is() const {
			if(typename Message::Magic_Const() != magic)
				return false;

			return typename Message::ID_Const() == id;
		}
	};

	/**
	 * A message header.
	 *
	 * \tparam ID Message type code.
	 * \tparam MAGIC Message magic.
	 * \tparam Payload Inherited payload class.
	 */
	template <std::uint8_t ID, std::uint32_t MAGIC, class Payload>
	struct Message {
		// Expose magic and this message type ID as constants.
		using Magic_Const = std::integral_constant<decltype(MAGIC), MAGIC>;
		using ID_Const = std::integral_constant<decltype(ID), ID>;

		WireMessageHeader header { MAGIC, ID };

		bool Read(binproto::BufferReader& reader) {
			// The header can be read first by an application then verified
			if(!header.read) {
				try {
					if(!header.Read(reader))
						return false;
				} catch(std::exception& ex) {
					return false;
				}
			}

			if(header.magic != MAGIC)
				return false;

			if(header.id != ID)
				return false;

			// This is kiiinda crusty.. but whatever
			try {
				if(!CRTPHelper()->ReadPayload(reader))
					return false;
			} catch(std::exception& ex) {
				return false;
			}
			return true;
		}

		void Write(binproto::BufferWriter& writer) const {
			header.Write(writer);

			// call the payload write function
			CRTPHelper()->WritePayload(writer);
		}

	   private:
		/**
		 * Retreive a pointer to the payload type.
		 */
		constexpr Payload* CRTPHelper() const {
			return (Payload*)this;
		}
	};

} // namespace binproto

#endif //BINPROTO_MESSAGE_H
