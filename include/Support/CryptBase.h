#pragma once

#ifndef COINBILL_SUPPORT_CRYPT_BASE
#define COINBILL_SUPPORT_CRYPT_BASE

// SHA / RSA Headers.

#include <vector>
#include <Support/CryptType.h>

namespace CoinBill
{
    void InitCryption();
    void StopCryption();

    typedef class __CRYPT_HANDLE*   CRYPT_HANDLE;

    typedef CRYPT_HANDLE            SHA256_HANDLE;
    typedef CRYPT_HANDLE            SHA512_HANDLE;

    typedef CRYPT_HANDLE            RSA_HANDLE;
    typedef RSA_HANDLE              RSAPUB_HANDLE;
    typedef RSA_HANDLE              RSAPRV_HANDLE;

    bool isSHA256HashEqual(SHA256_t& LHS, SHA256_t& RHS);
    bool isSHA512HashEqual(SHA512_t& LHS, SHA512_t& RHS);

    // key, hash object allocator.
    //
    SHA256_t* createSHA256();
    SHA512_t* createSHA512();
    RSA_t* createRSA();

    // key, hash object disposer.
    //
    inline void disposeSHA256(SHA256_t* object);
    inline void disposeSHA512(SHA512_t* object);
    inline void disposeRSA(RSA_t* object);
        
    // RSA - 4098 Binding.
    inline void queryRSAEnginePub(RSAPUB_HANDLE& handle, short PubExp, RSA_t& Module);
    inline void queryRSAEnginePrv(RSAPRV_HANDLE& handle, short PubExp, RSA_t& Module, RSA_t& PrvKey);
    inline void queryRSADeletePub(RSA_HANDLE& handle);
    inline void queryRSADeletePrv(RSA_HANDLE& handle);
    inline void queryRSAEncrypt(RSA_HANDLE& handle, void* pOut, void* pIn, size_t szIn);
    inline void queryRSADecrypt(RSA_HANDLE& handle, void* pOut, void* pIn, size_t szIn);
    inline void queryRSACreateKey(RSA_HANDLE& handle, RSA_t* Prv, RSA_t* Mod);

    // SHA-3 256 Hash Binding.
    //
    inline void querySHA256Engine(SHA256_HANDLE& handle);
    inline void querySHA256Verify(SHA256_HANDLE& handle, SHA256_t& out);
    inline void querySHA256Delete(SHA256_HANDLE& handle);

    // SHA-3 512 Hash Binding.
    //
    inline void querySHA512Engine(SHA512_HANDLE& handle);
    inline void querySHA512Verify(SHA512_HANDLE& handle, SHA512_t& out);
    inline void querySHA512Delete(SHA512_HANDLE& handle);
    

    // Update Function
    //
    inline void querySHAFlush(CRYPT_HANDLE& handle);
    inline void querySHAUpdate(CRYPT_HANDLE& handle, void* pIn, size_t szIn);

    template <class Ty>
    void querySHAUpdate(CRYPT_HANDLE& handle, Ty* pIn) {
        querySHAUpdate(handle, (void*)pIn, sizeof(Ty));
    }
    template <class Ty>
    void querySHAUpdate(CRYPT_HANDLE& handle, Ty& pIn) {
        querySHAUpdate(handle, (void*)&pIn, sizeof(Ty));
    }
    template <class Ty, size_t size>
    void querySHAUpdate(CRYPT_HANDLE& handle, Ty (&pIn)[size]) {
        querySHAUpdate(handle, (void*)pIn, sizeof(Ty * size));
    }
} // end namespace CoinBill

#endif // end COINBILL_SUPPORT_CRYPT_BASE