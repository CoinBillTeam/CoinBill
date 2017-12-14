#ifndef COINBILL_SUPPORT_BASIC
#define COINBILL_SUPPORT_BASIC

#include <iostream>
#include <sstream>

#define IF_FAILED(xv, y)                        if(!xv)                t
#define IF_FAILED_RET(xv, y)                    if(!xv)                return y;
#define IF_FAILED_GOTO(xv, y)                   if(!xv)                goto y;

#if defined(_MSC_VER)
#define ALIGNED_(x) __declspec(align(x))
#elif defined(__GNUC__)
#define ALIGNED_(x) __attribute__ ((aligned(x)))
#endif

#define ALIGN_V(ty, val)                        ALIGNED_(val)          ty
#define ALIGN_V_TYP(ty, typ)                    ALIGNED_(sizeof(typ))  ty
#define ALIGN_V_BIT(ty, bit)                    ALIGNED_(bit / 8)      ty

#if defined(_MSC_VER)
#define NOVTABLE __declspec(novtable)
#elif
#define NOVTABLE __attribute__ ((novtable))
#endif

#if _DEBUG && (defined(COINBILL_MAC_OS) || defined(COINBILL_UNIX_OS))
#  define COINBILL_ASSERT(exp) {                                  \
    if (!(exp)) {                                                 \
      std::ostringstream oss;                                     \
      oss << "Assertion failed: " << (char*)(__FILE__) << "("     \
          << (int)(__LINE__) << "): " << (char*)(__func__)        \
          << std::endl;                                           \
      std::cerr << oss.str();                                     \
      raise(SIGTRAP);                                             \
    }                                                             \
  }
#elif _DEBUG && defined(COINBILL_WINDOWS)
#  define COINBILL_ASSERT(exp) {                                  \
    if (!(exp)) {                                                 \
      std::ostringstream oss;                                     \
      oss << "Assertion failed: " << (char*)(__FILE__) << "("     \
          << (int)(__LINE__) << "): " << (char*)(__FUNCTION__)    \
          << std::endl;                                           \
      std::cerr << oss.str();                                     \
      __debugbreak();                                             \
    }                                                             \
  }
#endif // DEBUG and Unix or Windows

#ifndef COINBILL_ASSERT
#  define COINBILL_ASSERT(exp) ((void)(exp))
#endif

namespace CoinBill
{
    template <class Type> inline void* offset(Type* value, int index) {
        return (void*)((size_t)(value) + index);
    }

    // TODO : Use SIMD instructions for faster checks.
    template <class Type, size_t cycle>
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