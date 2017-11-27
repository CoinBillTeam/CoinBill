#ifndef COINBILL_SUPPORT_BIG_INT
#define COINBILL_SUPPORT_BIG_INT

#include <type_traits>
#include <stdint.h>
#include <Support/Basic.h>

#ifdef COINBILL_USE_SIMD
#include <immintrin.h>
#endif

namespace CoinBill
{
    // TODO : Maybe we can handle those types more gentely... those are not really good implements.

    template <unsigned int size, class BaseTy = unsigned char, BaseTy max_size = (BaseTy)(-1)>
    class CryptType
    {
        // NOTE : DO NOT CREATE ANY VIRTUAL METHODS, OR ANY OTHER VARIABLES.
        //        THIS IS FOR STORING RAW BIG INT VARIABLES.
    protected:
        using MTy = CryptType<size, BaseTy>;
        BaseTy data[size];

        void ZeroFill();

        void increasePos(unsigned int index, BaseTy val) {
            // If overflow.
            if (data[index] < max_size - val) {
                increasePos(index + 1, 1);
                data[index] = max_size - (val + data[index]);
            }
            else
                data[index] += val;
        }
        void decreasePos(unsigned int index, BaseTy val) {
            // If underflow.
            if (data[index] < val) {
                decreasePos(index + 1, 1);
                data[index] = max_size - (val - data[index]);
            }
            else
                data[index] -= val;
        }

    public:
        inline friend bool operator==(const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) == 0; }
        inline friend bool operator!=(const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) != 0; }
        inline friend bool operator< (const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) <  0; }
        inline friend bool operator<=(const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) <= 0; }
        inline friend bool operator> (const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) >  0; }
        inline friend bool operator>=(const MTy& LHS, const MTy& RHS) { return iterate_cmp<BaseTy, size>((void*)LHS.data, (void*)RHS.data) >= 0; }
        inline operator BaseTy*() { return data; }
        inline operator void*() { return data }
        inline operator unsigned char*() { return toType<uint8_t>(); }
        inline operator char*() { return toType<int8_t>(); }

        template <class OTy> inline MTy& operator++(OTy) { 
            increasePos(0, (BaseTy)1); 
            return *this;
        }
        template <class OTy> inline MTy& operator+(OTy val) { 
            increasePos(0, (BaseTy)val); 
            return *this;
        }
        template <class OTy> inline MTy& operator--(OTy) { 
            decreasePos(0, (BaseTy)1); 
            return *this;
        }
        template <class OTy> inline MTy& operator-(OTy val) { 
            decreasePos(0, (BaseTy)val); 
            return *this;
        }

        template <class OTy> inline MTy& operator=(OTy val) {
            ZeroFill();
            OTy* v = toType<OTy>();
            *v = (OTy)val;
            return *this;
        }

        template <class OTy>
        friend OTy operator%(const MTy& LHS, const OTy RHS) {
            return LHS.data[0] % RHS;
        }

        size_t getSize() { return sizeof(BaseTy) * size; }

        template <class Ty, unsigned int szToTy = (sizeof(BaseTy) * size) / sizeof(Ty)>
        constexpr Ty* toType(unsigned int& _size) {
            _size = szToTy; return (Ty*)data;
        }
        template <class Ty>
        constexpr Ty* toType() {
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

        bool isEmpty();

        // custom type constructor.
        template<class Ty>
        CryptType(const Ty Init) {
            *this = Init;
        }
        
        // default type constructor / distructor.
        CryptType() = default;
        ~CryptType() = default;

        uint8_t* toUint8(unsigned int& _size) { return toType<uint8_t>(_size); }
        uint16_t* toUint16(unsigned int& _size) { return toType<uint16_t>(_size); }
        uint32_t* toUint32(unsigned int& _size) { return toType<uint32_t>(_size); }
        uint64_t* toUint64(unsigned int& _size) { return toType<uint64_t>(_size); }

        uint8_t* toUint8() { return toType<uint8_t>(); }
        uint16_t* toUint16() { return toType<uint16_t>(); }
        uint32_t* toUint32() { return toType<uint32_t>(); }
        uint64_t* toUint64() { return toType<uint64_t>(); }
    };

    typedef CryptType<2, uint64_t>                                  uint128_t;
    typedef CryptType<4, uint64_t>                                  uint256_t;
    typedef CryptType<8, uint64_t>                                  uint512_t;
    typedef CryptType<64, uint64_t>                                 uint4096_t;

    typedef uint4096_t                                              RSA_t;
    typedef uint256_t                                               SHA256_t;
    typedef uint512_t                                               SHA512_t;

    template <unsigned int size, class BaseTy, BaseTy maxSize>
    inline bool CryptType<size, BaseTy, maxSize>::isEmpty() {
        for (unsigned int i = 0; i < size; ++i)
            if (data[i] != 0)
                return false;
        return true;
    }

    template <unsigned int size, class BaseTy, BaseTy maxSize>
    inline void CryptType<size, BaseTy, maxSize>::ZeroFill() {
        for (unsigned int i = 0; i < size; ++i) data[i] = 0;
    }

#ifdef COINBILL_USE_SIMD
    //      ----=========================================================================================================----
    //          |        SIMD Implements With Template Specalization.          using SIMD for better performance.       |
    //      ----=========================================================================================================----
    //          | Targets                                                   |                                           |
    //          |   - uint2048_t                                            | RSA2048_t                                 |
    //          |   - uint256_t                                             | SHA256_t                                  |
    //          |   - uint512_t                                             | SHA512_t                                  |
    //      ----=========================================================================================================----
    //          | bool isEmpty()                                                                                        |
    //          |   - empty check for value.                                                                            |
    //          |                                                                                                       |
    //      ----=========================================================================================================----
    //          | void ZeroFill()                                                                                       |
    //          |   - set values empty.                                                                                 |
    //          |                                                                                                       |
    //      ----=========================================================================================================----
    //          | Ty& operator=(Ty&)                                                                                    |
    //          |   - copy values.                                                                                      |
    //          |                                                                                                       |
    //      ----=========================================================================================================----
    //
    //
    template <>
    inline bool uint256_t::isEmpty() {
        // Load to register.
        __m256i v = _mm256_load_si256(toType<__m256i>());

        // check zero flag from simd register.
        // _mm256_testz_si256(v, v) => 
        //      (v[255:0] & v[255:0] == 0) return 1;
        return !!_mm256_testz_si256(v, v);
    }

    template<>
    inline bool uint512_t::isEmpty() {
        // Load to register.
        // We are not just going to use the loop
        // Better just hardcode it...
        __m256i* tmpV = toType<__m256i>();
        __m256i v1 = _mm256_load_si256(&tmpV[0]);
        __m256i v2 = _mm256_load_si256(&tmpV[1]);

        // check zero flag from simd register.
        __m256i result = _mm256_or_si256(v1, v2);
        return !!_mm256_testz_si256(result, result);
    }

    template<>
    inline bool uint4096_t::isEmpty() {
        unsigned int indexV;
        __m256i* tempV = toType<__m256i>(indexV);

        // I think... loop unroll shoud be performed in here.
        // maybe msvc will not unroll this. please check this code can unrollable.
        int no_zf_stood = 0;
        for (unsigned int i = 0; i < indexV; ++i) {
            // Load and tests.
            // We do not use branchs, non-branch loop is lot faster for this.
            __m256i v = _mm256_load_si256(&tempV[i]);
            no_zf_stood |= !_mm256_testz_si256(v, v);
        }
        return !no_zf_stood;
    }

    template<>
    inline void uint256_t::ZeroFill() {
        // Load to register.
        __m256i* vo = toType<__m256i>();
        // Store a zero value.
        _mm256_store_si256(vo, _mm256_setzero_si256());
    }

    template<>
    inline void uint512_t::ZeroFill() {
        // Load to register.
        __m256i* vo = toType<__m256i>();
        const __m256i vz = _mm256_setzero_si256();
        // Store a zero value.

        _mm256_store_si256(&vo[0], vz);
        _mm256_store_si256(&vo[1], vz);
    }

    template<>
    inline void uint4096_t::ZeroFill() {
        // Load to register.
        unsigned int vi;
        __m256i* vo = toType<__m256i>(vi);

        const __m256i vz = _mm256_setzero_si256();
        for (unsigned int i = 0; i < vi; ++i) {
            _mm256_store_si256(&vo[i], vz);
        }
    }

    template<>
    template<>
    inline uint256_t& uint256_t::operator=<uint256_t&>(uint256_t& val) {
        __m256i* vl = toType<__m256i>();
        __m256i* vr = val.toType<__m256i>();

        _mm256_store_si256(vl, _mm256_load_si256(vr));
        return *this;
    }

    template<>
    template<>
    inline uint512_t& uint512_t::operator=<uint512_t&>(uint512_t& val) {
        __m256i* vl = toType<__m256i>();
        __m256i* vr = val.toType<__m256i>();


        _mm256_store_si256(&vl[0], _mm256_load_si256(&vr[0]));
        _mm256_store_si256(&vl[1], _mm256_load_si256(&vr[1]));
        return *this;
    }

    template<>
    template<>
    inline uint4096_t& uint4096_t::operator=<uint4096_t&>(uint4096_t& val) {
        unsigned int vi;
        __m256i* vl = toType<__m256i>(vi);
        __m256i* vr = val.toType<__m256i>();

        for (unsigned int i = 0; i < vi; ++i) {
            _mm256_store_si256(&vl[i], _mm256_load_si256(&vr[i]));
        }
        return *this;
    }
#endif // COINBILL_USE_SIMD
}

#endif // COINBILL_SUPPORT_BIG_INT
