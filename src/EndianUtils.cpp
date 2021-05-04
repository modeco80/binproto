#include <binproto/EndianUtils.h>

#ifdef _MSC_VER
	#if defined(_M_IX86) || defined(_M_AMD64)
		#define BINPROTO_IS_X86
	#endif

	// On MSVC, we rely on the LE_ISX86 define
	// so placate the GNU stuff...
	#define __BYTE_ORDER__ 0
	#define __ORDER_LITTLE_ENDIAN__ 1000
	#define __ORDER_BIG_ENDIAN__ 0001
#endif

// If any of these cases are true we are compiling for a little endian environment
#if defined(BINPROTO_IS_X86) || __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	#define LE
#endif

namespace binproto::internal {

	template <class T>
	constexpr T& PtrAs(void* ptr) {
		return *(T*)ptr;
	}

	template <class T>
	constexpr const T& PtrAs(const void* ptr) {
		return *(T*)ptr;
	}

	template <>
	std::uint16_t ReadBE<std::uint16_t>(const std::uint8_t* base) {
		const auto& i = PtrAs<std::uint16_t>(base);

#ifdef LE
		return __builtin_bswap16(i);
#else
		return i;
#endif
	}

	template <>
	void WriteBE<std::uint16_t>(std::uint8_t* base, const std::uint16_t& val) {
		auto& i = PtrAs<std::uint16_t>(base);
#ifdef LE
		i = __builtin_bswap16(val);
#else
		i = val;
#endif
	}

	template <>
	std::uint32_t ReadBE<std::uint32_t>(const std::uint8_t* base) {
		const auto& i = PtrAs<std::uint32_t>(base);
#ifdef LE
		return __builtin_bswap32(i);
#else
		return i;
#endif
	}

	template <>
	void WriteBE<std::uint32_t>(std::uint8_t* base, const std::uint32_t& val) {
		auto& i = PtrAs<std::uint32_t>(base);
#ifdef LE
		i = __builtin_bswap32(val);
#else
		i = val;
#endif
	}

	template <>
	std::uint64_t ReadBE<std::uint64_t>(const std::uint8_t* base) {
		const auto& i = PtrAs<std::uint64_t>(base);
#ifdef LE
		return __builtin_bswap64(i);
#else
		return i;
#endif
	}

	template <>
	void WriteBE<std::uint64_t>(std::uint8_t* base, const std::uint64_t& val) {
		auto& i = PtrAs<std::uint64_t>(base);
#ifdef LE
		i = __builtin_bswap64(val);
#else
		i = val;
#endif
	}

} // namespace binproto::internal