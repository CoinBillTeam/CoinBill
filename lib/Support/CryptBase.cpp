#include <memory>

#include <Support/CryptBase.h>
#include <Support/CryptType.h>
#include <Support/ResourcePool.h>

#include <iostream>
#include <sstream>


// CryptoPP headers.
#include <rsa.h>
#include <sha3.h>
#include <rng.h>

CryptoPP::RandomNumberGenerator* RNG_Engine;

namespace CoinBill
{                   
    ResourcePool<SHA256_t>                          SHA256_MemPool;
    ResourcePool<CryptoPP::SHA3_256>                SHA256Engine_MemPool;

    ResourcePool<SHA512_t>                          SHA512_MemPool;
    ResourcePool<CryptoPP::SHA3_512>                SHA512Engine_MemPool;

    ResourcePool<RSA_t>                             RSA_MemPool;
    ResourcePool<CryptoPP::InvertibleRSAFunction>   RSAEngine_MemPoolPrv;
    ResourcePool<CryptoPP::RSAFunction>             RSAEngine_MemPoolPub;

    // Mananged key, hash object allocators.
    // we do allocate key, hash holder from MemPool class for faster allocation.
    // do not deallocate objects in other place, use disposeXXX function.
    //
    // those objects are thread-safe, mutex is inside in ResourcePool class.
    // you don't need to care about thread safes.
    SHA256_t* createSHA256()                    { return new (SHA256_MemPool) SHA256_t(); }
    SHA512_t* createSHA512()                    { return new (SHA512_MemPool) SHA512_t(); }
    RSA_t* createRSA()                          { return new (RSA_MemPool) RSA_t(); }

    // Managed key, hash object disposer.
    void disposeSHA256(SHA256_t* object)        { operator delete(object, SHA256_MemPool); }
    void disposeSHA512(SHA512_t* object)        { operator delete(object, SHA512_MemPool); }
    void disposeRSA(RSA_t* object)              { operator delete(object, RSA_MemPool); }

    void InitCryption() {
        RNG_Engine = new CryptoPP::RandomNumberGenerator();
    }
    void StopCryption() {
        delete RNG_Engine;
    }

    /// TODO : is there are better way to check is equal using SIMD..
    ///        Maybe we can use XOR with it.
#ifdef COINBILL_USE_AVX2
    bool isSHA256HashEqual(SHA256_t& LHS, SHA256_t& RHS) {
        __m256i vl = _mm256_load_si256(LHS.toType<__m256i>());
        __m256i vr = _mm256_load_si256(RHS.toType<__m256i>());
        __m256i result = _mm256_xor_si256(vl, vr);

        // XOR result should zero, so there is no diffrence between LHS and RHS.
        // we are checking that result is zero by testing bits. if the zero flag is 1 means zero.
        return !_mm256_testz_si256(result, result);
    }
    bool isSHA512HashEqual(SHA512_t& LHS, SHA512_t& RHS) {
        __m256i* pvl = LHS.toType<__m256i>();
        __m256i* pvr = RHS.toType<__m256i>();

        // xor LHS, RHS. if result of xor is zero, its equal.
        __m256i masked1 = _mm256_xor_si256(
            _mm256_load_si256(&pvl[0]),
            _mm256_load_si256(&pvr[0])
        );
        __m256i masked2 = _mm256_xor_si256(
            _mm256_load_si256(&pvl[1]),
            _mm256_load_si256(&pvr[1])
        );

        // or the xor result.
        __m256i result = _mm256_or_si256(masked1, masked2);
        return  !_mm256_testz_si256(result, result);
    }
#else  // COINBILL_USE_SIMD
    bool isSHA256HashEqual(SHA256_t& LHS, SHA256_t& RHS) {
        return LHS == RHS;
    }
    bool isSHA512HashEqual(SHA512_t& LHS, SHA512_t& RHS) {
        return LHS == RHS;
    }
#endif // !COINBILL_USE_SIMD

    // creating a instance from memory pool.
    // the raw class instance. we are going to handle it as just a pointer.
    // because we want to do cryption only on here.
    // for easier debugging, logging, avoid deallocating in other place.
    //
    // these objects are managed by MemPool class for faster allocation.
    // do not deallocate manually.
    //
    // This isn't a SHA256 object itself, its engine that we can compute cryption.
    void querySHA256Engine(SHA256_HANDLE& handle) {
        // allocate from SHA256 Engine pool.
        handle = (SHA256_HANDLE)(new(SHA256Engine_MemPool) CryptoPP::SHA3_256());
    }
    void querySHA512Engine(SHA512_HANDLE& handle) {
        // allocate from SHA512 Engine pool.
        handle = (SHA512_HANDLE)(new(SHA512Engine_MemPool) CryptoPP::SHA3_512());
    }
    void queryRSAEnginePub(RSAPUB_HANDLE& handle, short PubExp, RSA_t& Module) {
        CryptoPP::RSAFunction *Func = new(RSAEngine_MemPoolPub) CryptoPP::RSAFunction();
        Func->Initialize
        (
            std::move(CryptoPP::Integer(Module, Module.getSize(), CryptoPP::Integer::UNSIGNED, CryptoPP::BIG_ENDIAN_ORDER)),
            std::move(CryptoPP::Integer(3))
        );
        handle = (RSAPUB_HANDLE)(Func);
    }
    void queryRSAEnginePrv(RSAPRV_HANDLE& handle, short PubExp, RSA_t& Module, RSA_t& PrvKey) {
        CryptoPP::InvertibleRSAFunction *Func = new(RSAEngine_MemPoolPrv) CryptoPP::InvertibleRSAFunction();
        Func->Initialize
        (
            std::move(CryptoPP::Integer(Module, Module.getSize(), CryptoPP::Integer::UNSIGNED, CryptoPP::BIG_ENDIAN_ORDER)),
            std::move(CryptoPP::Integer(3)),
            std::move(CryptoPP::Integer(PrvKey, PrvKey.getSize(), CryptoPP::Integer::UNSIGNED, CryptoPP::BIG_ENDIAN_ORDER))
            
        );
        handle = (RSAPRV_HANDLE)(Func);
    }

    // add a crypt target in a instance.
    // casing handle into a raw class instance.
    // and we are going to update the target buffer on it before we finalize it.
    //
    // note that you should call flush method before reusing it.
    void querySHAUpdate(CRYPT_HANDLE& handle, void* pIn, size_t szIn) {
        CryptoPP::SHA3*
            pEngine = (CryptoPP::SHA3*)handle;
            pEngine->Update((uint8_t*)pIn, szIn);
        // Add crypt target in engine.
    }

    // finalize crypt instance.
    // we create a actaul hash here. out buffer should not be nullptr.
    void querySHA256Verify(SHA256_HANDLE& handle, SHA256_t& out) {
        auto*
            pEngine = (CryptoPP::SHA3_256*)handle;
            pEngine->Final(out);
    }
    void querySHA512Verify(SHA512_HANDLE& handle, SHA512_t& out) {
        auto*
            pEngine = (CryptoPP::SHA3_512*)handle;
            pEngine->Final(out);
    }

    // deleting, destructing instance.
    // we distruct the instance and recollect into MemPool.
    // 
    // we don't really recommend if you are reusing a object, use flush instead of this.
    void querySHA256Delete(SHA256_HANDLE& handle) {
        auto* pEngine = (CryptoPP::SHA3_256*)handle;
        operator delete(pEngine, SHA256_MemPool);
    }
    void querySHA512Delete(SHA512_HANDLE& handle) {
        auto* pEngine = (CryptoPP::SHA3_512*)handle;
        operator delete(pEngine, SHA512_MemPool);
    }
    void queryRSADeletePub(RSA_HANDLE& handle) {
        auto* pEngine = (CryptoPP::RSAFunction*)handle;
        operator delete(pEngine, RSAEngine_MemPoolPub);
    }
    void queryRSADeletePrv(RSA_HANDLE& handle) {
        auto* pEngine = (CryptoPP::InvertibleRSAFunction*)handle;
        operator delete(pEngine, RSAEngine_MemPoolPrv);
    }

    // flush instance.
    // flush instance values, that stored using verifiy, update function.
    void querySHA256Flush(SHA256_HANDLE& handle) {
        auto* pEngine = (CryptoPP::SHA3_256*)handle;
        pEngine->Restart();
    }
    void querySHA512Flush(SHA512_HANDLE& handle) {
        auto* pEngine = (CryptoPP::SHA3_256*)handle;
        pEngine->Restart();
    }
    
    // Encrypt, Decrypt functions.
    // basically, we use these functions for signing.
    void queryRSAEncrypt(RSA_HANDLE& handle, void* pOut, void* pIn, size_t szIn) {
        auto* pEngine = (CryptoPP::TrapdoorFunction*)handle;

        CryptoPP::Integer Encrypted = pEngine->ApplyFunction
        (
            CryptoPP::Integer((const uint8_t*)pIn, szIn, CryptoPP::Integer::UNSIGNED, CryptoPP::BIG_ENDIAN_ORDER)
        );

        std::stringstream oBuf((char*)pOut);
        oBuf << Encrypted;
    }
    void queryRSADecrypt(RSA_HANDLE& handle, void* pOut, void* pIn, size_t szIn) {
        auto* pEngine = (CryptoPP::TrapdoorFunctionInverse*)handle;

        CryptoPP::Integer Round = pEngine->CalculateInverse
        (
            *RNG_Engine,
            CryptoPP::Integer((const uint8_t*)pIn, szIn, CryptoPP::Integer::UNSIGNED, CryptoPP::BIG_ENDIAN_ORDER)
        );
        Round.Encode((uint8_t*)pOut, Round.ByteCount());
    }

    void queryRSACreateKey(RSA_HANDLE& handle, RSA_t* Prv, RSA_t* Mod) {
        auto* pEngine = (CryptoPP::InvertibleRSAFunction*)handle;

        pEngine->GenerateRandomWithKeySize(*RNG_Engine, sizeof(RSA_t) * 8);

        std::stringstream prvBuf((char*)Prv);
        std::stringstream modBuf((char*)Mod);

        prvBuf << pEngine->GetPrivateExponent();
        modBuf << pEngine->GetModulus();
    }
}