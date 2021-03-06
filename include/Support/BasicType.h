#ifndef COINBILL_SUPPORT_BASIC_TYPE
#define COINBILL_SUPPORT_BASIC_TYPE

#include <Support/BasicUtility.h>
#include <Support/ResourcePool.h>

#include <type_traits>
#include <stdint.h>
#include <stdlib.h>

#ifdef COINBILL_USE_AVX2
#include <intrin.h>
#endif

// We are disabling 4091 warning.
// extern will help you to make compile time lot more faster.
#pragma warning(disable:4091)

#define BIG_INTEGER_TYPE(size)                      \
    typedef BIType<size> uint##size##_t;            \
    extern typename uint##size##_t

namespace CoinBill
{
    // BasicType for creating custom bit integer type.
    template <uint64_t bits>
    class BIType {
        typedef BIType<bits> MTy;
		static thread_local ResourcePool<MTy> Pool;

    public:
        static constexpr size_t in_bytes    = bits / 8;
        static constexpr size_t u8_sz       = in_bytes / sizeof(uint8_t);
        static constexpr size_t u16_sz      = in_bytes / sizeof(uint16_t);
        static constexpr size_t u32_sz      = in_bytes / sizeof(uint32_t);
        static constexpr size_t u64_sz      = in_bytes / sizeof(uint64_t);
		

        union { 
            uint8_t u8[u8_sz];
            uint16_t u16[u16_sz];
            uint32_t u32[u32_sz];
            uint64_t u64[u64_sz];
        };

        MTy& operator~();
        MTy& operator&(MTy& LHS);
        MTy& operator|(MTy& LHS);
        MTy& operator^(MTy& LHS);
		MTy& operator=(MTy& RHS);
        MTy& operator=(uint64_t value);
		
		bool operator==(MTy& LHS);
		bool operator<=(MTy& LHS);

        BIType(const uint64_t value) { 
            *this = value; 
        }
        BIType(const BIType& init) : BIType(0) { }
		BIType() = default;

		// custom fast allocations.
		// these are thread local, so you don't need to take care about syncronization.
		// but remember, this will make a huge memory allocations by allocating pools.
		inline void* operator new(size_t size) {
			return Pool.create();
		}
		inline void operator delete(void* object, size_t size) {
			Pool.distroy((MTy*)object);
		}
    };

    template<uint64_t bits>
    inline BIType<bits>& BIType<bits>::operator~() {
        for (unsigned int i = 0; i < u64_sz; ++i)
            u64[i] = ~u64[i];
        return *this;
    }
    template<uint64_t bits>
    inline BIType<bits>& BIType<bits>::operator&(MTy& LHS) {
        for (unsigned int i = 0; i < u64_sz; ++i)
            u64[i] = u64[i] & LHS.u64[i];
        return *this;
    }
    template<uint64_t bits>
    inline BIType<bits>& BIType<bits>::operator|(MTy& LHS) {
        for (unsigned int i = 0; i < u64_sz; ++i)
            u64[i] = u64[i] | LHS.u64[i];
        return *this;
    }
    template<uint64_t bits>
    inline BIType<bits>& BIType<bits>::operator^(MTy& LHS) {
        for (unsigned int i = 0; i < u64_sz; ++i)
            u64[i] = u64[i] ^ LHS.u64[i];
        return *this;
    }

    template<uint64_t bits>
    inline BIType<bits>& BIType<bits>::operator=(uint64_t value) {
        *this = *this ^ *this; u64[1] = value;
        return *this;
    }

	template<uint64_t bits>
	inline BIType<bits>& BIType<bits>::operator=(MTy& RHS) {
		for (unsigned int i = 0; i < u64_sz; ++i)
			u64[i] = RHS.u64[i];
		return *this;
	}

	template<uint64_t bits>
	inline bool BIType<bits>::operator==(MTy& RHS) {
		return iterate_cmp<uint64_t>((void*)RHS.u64, (void*)u64, MTy::u64_sz) == 0;
	}

	template<uint64_t bits>
	inline bool BIType<bits>::operator<=(MTy& RHS) {
		return iterate_cmp<uint64_t>((void*)RHS.u64, (void*)u64, MTy::u64_sz) <= 0;
	}

    // Big Integer Defines, Those types will be uintXXXX_t.
    // XXXX is size of type.
    BIG_INTEGER_TYPE(128);  BIG_INTEGER_TYPE(192);  BIG_INTEGER_TYPE(256);
    BIG_INTEGER_TYPE(320);  BIG_INTEGER_TYPE(384);  BIG_INTEGER_TYPE(448);
    BIG_INTEGER_TYPE(512);  BIG_INTEGER_TYPE(1024); BIG_INTEGER_TYPE(2048);
    BIG_INTEGER_TYPE(3072); BIG_INTEGER_TYPE(4096); BIG_INTEGER_TYPE(8192);
}
#endif // COINBILL_SUPPORT_BASIC_TYPE