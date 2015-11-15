/**
 * @file include/kernel/endian.h
 * Kernel operations for translating endianness.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_KERNEL_ENDIAN_H_
#define __INCLUDE_KERNEL_ENDIAN_H_

#include <stdint.h>

#define __LITTLE_ENDIAN__ (1)    //!< Is this system little endian?
#undef  __BIG_ENDIAN__           //!< Is this system big endian?

static inline uint16_t endian_swap16(uint16_t x){
	return (x<<8)|(x>>8);
}

static inline unsigned int endian_swap32(unsigned int x){
	return (x<<24)|((x&0x0000FF00u)<<8)|((x&0x00FF0000u)>>8)|(x>>24);
}

static inline unsigned long long endian_swap64(unsigned long long x){
	return (x<<56)|((x&0x000000000000FF00ull)<<40)|((x&0x0000000000FF0000ull)<<24)|((x&0x00000000FF000000ull)<<8)|
			((x&0x000000FF00000000ull)>>8)|((x&0x0000FF0000000000)>>24)|((x&0x00FF000000000000)>>40)|(x>>56);
}

#ifdef __LITTLE_ENDIAN__
	static inline uint16_t endian_be16(uint16_t x){
		return endian_swap16(x);
	}

	static inline uint32_t endian_be32(uint32_t x){
		return endian_swap32(x);
	}

	static inline uint64_t endian_be64(uint64_t x){
		return endian_swap64(x);
	}

	static inline uint16_t endian_le16(uint16_t x){
		return x;
	}

	static inline uint32_t endian_le32(uint32_t x){
		return x;
	}

	static inline uint64_t endian_le64(uint64_t x){
		return x;
	}
#elif defined(__BIG_ENDIAN__)
	static inline uint16_t endian_be16(uint16_t x){
		return x;
	}

	static inline uint32_t endian_be32(uint32_t x){
		return x;
	}

	static inline uint64_t endian_be64(uint64_t x){
		return x;
	}

	static inline uint16_t endian_le16(uint16_t x){
		return endian_swap16(x);
	}

	static inline uint32_t endian_le32(uint32_t x){
		return endian_swap32(x);
	}

	static inline uint64_t endian_le64(uint64_t x){
		return endian_swap64(x);
	}
#else
	#error Endianness is undefined.
#endif

#endif /* __INCLUDE_KERNEL_ENDIAN_H_ */

