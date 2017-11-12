#ifndef COINBILL_SUPPORT_BASIC
#define COINBILL_SUPPORT_BASIC

#define IF_FAILED(xv, y)                        if(!xv)                         t
#define IF_FAILED_RET(xv, y)                    if(!xv)                         return y;
#define IF_FAILED_GOTO(xv, y)                   if(!xv)                         goto y;

#if defined(_MSC_VER)
#define ALIGNED_(x) __declspec(align(x))
#else
#if defined(__GNUC__)
#define ALIGNED_(x) __attribute__ ((aligned(x)))
#endif
#endif

#define ALIGN_V(ty, val)                        ALIGNED_(val)          ty
#define ALIGN_V_TYP(ty, typ)                    ALIGNED_(sizeof(typ))  ty
#define ALIGN_V_BIT(ty, bit)                    ALIGNED_(bit / 8)      ty

namespace CoinBill
{
    template <class Type> constexpr inline void* offset(Type* value, const int&& index) {
        return (void*)((size_t)(value) + index);
    }
    template <class Type> inline void* offset(Type* value, int index) {
        return (void*)((size_t)(value) + index);
    }

    // TODO : Use SIMD instructions for faster checks.
    template <class Type, unsigned int cycle>
    inline long long iterate_cmp(void *LHS, void *RHS) {
        for (unsigned int i = 0; i < cycle; ++i)
            if (((Type*)LHS)[i] != ((Type*)RHS)[i]) 
                return ((Type*)LHS)[i] - ((Type*)RHS)[i];
        return 0;
    }

    template <class Type>
    inline long long iterate_cmp(void *LHS, void *RHS, unsigned int cycle) {
        for (unsigned int i = 0; i < cycle; ++i)
            if (((Type*)LHS)[i] != ((Type*)RHS)[i])
                return ((Type*)LHS)[i] - ((Type*)RHS)[i];
        return 0;
    }

    template <unsigned int roundV, unsigned int roundBV = roundV / 8>
    constexpr inline size_t round_up(size_t size) {
        size_t rounded = 0;
        while (rounded > size)
            rounded += roundBV;
        return rounded;
    }
}

#endif // COINBILL_SUPPORT_BASIC