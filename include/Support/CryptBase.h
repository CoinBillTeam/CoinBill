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

    typedef void*                   CRYPT_HANDLE;

    typedef CRYPT_HANDLE            SHA256_HANDLE;
    typedef CRYPT_HANDLE            SHA512_HANDLE;

    typedef CRYPT_HANDLE            RSA_HANDLE;
    typedef RSA_HANDLE              RSAPUB_HANDLE;
    typedef RSA_HANDLE              RSAPRV_HANDLE;

    inline bool isSHA256HashEqual(SHA256_t& LHS, SHA256_t& RHS) {
        return true;
    }
    inline bool isSHA512HashEqual(SHA512_t& LHS, SHA512_t& RHS) {
        return true;
    }

    // key, hash object allocator.
    //
    SHA256_t* createSHA256();
    SHA512_t* createSHA512();
    RSA_t* createRSA();

    // key, hash object disposer.
    //
    void disposeSHA256(SHA256_t* object);
    void disposeSHA512(SHA512_t* object);
    void disposeRSA(RSA_t* object);
        
    // RSA - 4098 Binding.
    void queryRSAEnginePub(RSAPUB_HANDLE& handle, short PubExp, RSA_t& Module);
    void queryRSAEnginePrv(RSAPRV_HANDLE& handle, short PubExp, RSA_t& Module, RSA_t& PrvKey);
    void queryRSADeletePub(RSA_HANDLE& handle);
    void queryRSADeletePrv(RSA_HANDLE& handle);
    void queryRSAEncrypt(RSA_HANDLE& handle, void* pOut, void* pIn, size_t szIn);
    void queryRSADecrypt(RSA_HANDLE& handle, void* pOut, void* pIn, size_t szIn);
    void queryRSACreateKey(RSA_HANDLE& handle, RSA_t* Prv, RSA_t* Mod);

    // SHA-3 256 Hash Binding.
    //
    void querySHA256Engine(SHA256_HANDLE& handle);
    void querySHA256Verify(SHA256_HANDLE& handle, SHA256_t& out);
    
    // SHA-3 512 Hash Binding.
    //
    void querySHA512Engine(SHA512_HANDLE& handle);
    void querySHA512Verify(SHA512_HANDLE& handle, SHA512_t& out);

    // Update Function
    //
    void querySHAFlush(CRYPT_HANDLE& handle);
    void querySHAUpdate(CRYPT_HANDLE& handle, void* pIn, size_t szIn);
    void querySHADelete(CRYPT_HANDLE& handle);

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