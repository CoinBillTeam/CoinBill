#ifndef COINBILL_SUPPORT_BIG_INT
#define COINBILL_SUPPORT_BIG_INT

#include <stdint.h>
#include <Support/Basic.h>

namespace CoinBill
{
    
    template <unsigned int size, class BaseTy = char>
    class BigInt
    {
        // NOTE : DO NOT CREATE ANY VIRTUAL METHODS, OR ANY OTHER VARIABLES.
        //        THIS IS FOR STORING RAW BIG INT VARIABLES.

    protected:
        typedef BigInt<size, BaseTy> MTy;
        BaseTy data[size];

    public:
        inline friend bool operator==(const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) == 0; }
        inline friend bool operator!=(const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) != 0; }
        inline friend bool operator< (const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) <  0; }
        inline friend bool operator<=(const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) <= 0; }
        inline friend bool operator> (const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) >  0; }
        inline friend bool operator>=(const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) >= 0; }
        inline operator BaseTy*() { return data; }
        inline operator void*() { return (void*)data; }

        size_t getSize() { return sizeof(BaseTy) * size; }

        template <class Ty, unsigned int szToTy = (sizeof(BaseTy) * size) / sizeof(Ty)>
        Ty* toType(unsigned int& _size) {
            _size = szToTy; return (Ty*)data;
        }
        template <class Ty>
        Ty* toType() {
            return (Ty*)data;
        }

        template <class Ty, size_t szTy>
        void toArray(Ty (&Arr)[szTy]) {
            static_assert(sizeof(Ty) * szTy > sizeof(BaseTy) * size);
            Ty* Tmp = toType<Ty>();
            for (unsigned int i = 0; i != szTy; ++i) {
                Arr[i] = Tmp[i];
            }
        }

        uint8_t* toUint8(unsigned int& _size) { return toType<uint8_t>(_size); }
        uint16_t* toUint16(unsigned int& _size) { return toType<uint16_t>(_size); }
        uint32_t* toUint32(unsigned int& _size) { return toType<uint32_t>(_size); }
        uint64_t* toUint64(unsigned int& _size) { return toType<uint64_t>(_size); }

        uint8_t* toUint8() { return toType<uint8_t>(); }
        uint16_t* toUint16() { return toType<uint16_t>(); }
        uint32_t* toUint32() { return toType<uint32_t>(); }
        uint64_t* toUint64() { return toType<uint64_t>(); }

        // I think... we can make this lot more faster tho.
        void ZeroFill() {
            for (unsigned int i = 0; i < size; ++i) data[i] = 0;
        }
    };

    typedef BigInt<2, uint64_t>                                     uint128_t;
    typedef BigInt<4, uint64_t>                                     uint256_t;
    typedef BigInt<8, uint64_t>                                     uint512_t;

    typedef uint256_t                                               SHA256_t;
    typedef uint512_t                                               SHA512_t;
    typedef uint256_t                                               RSA256_t;
    typedef uint512_t                                               RSA512_t;
}

#endif