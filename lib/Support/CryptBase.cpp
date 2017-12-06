#include <memory>

#include <Support/CryptBase.h>
#include <Support/CryptType.h>
#include <Support/ResourcePool.h>

#include <iostream>
#include <sstream>

#include <botan/sha3.h>

namespace CoinBill
{                   
    ResourcePool<SHA256_t>                          SHA256_MemPool;
    ResourcePool<SHA512_t>                          SHA512_MemPool;
    ResourcePool<RSA_t>                             RSA_MemPool;

    ResourcePool<Botan::SHA_3_256>                  SHA256Module_Pool;
    ResourcePool<Botan::SHA_3_384>                  SHA384Module_Pool;
    ResourcePool<Botan::SHA_3_512>                  SHA512Module_Pool;

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
    }
    void StopCryption() {
    }

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
        handle = (SHA256_HANDLE)(new (SHA256Module_Pool) Botan::SHA_3_256());
    }
    void querySHA512Engine(SHA512_HANDLE& handle) {
        handle = (SHA512_HANDLE)(new (SHA512Module_Pool) Botan::SHA_3_512());
    }
    void queryRSAEnginePub(RSAPUB_HANDLE& handle, short PubExp, RSA_t& Module) {
    }
    void queryRSAEnginePrv(RSAPRV_HANDLE& handle, short PubExp, RSA_t& Module, RSA_t& PrvKey) {
    }

    // add a crypt target in a instance.
    // casing handle into a raw class instance.
    // and we are going to update the target buffer on it before we finalize it.
    //
    // note that you should call flush method before reusing it.
    void querySHAUpdate(CRYPT_HANDLE& handle, void* pIn, size_t szIn) {
        auto* Module = (Botan::HashFunction*)(handle);
        Module->update((uint8_t*)pIn, szIn);
    }

    // finalize crypt instance.
    // we create a actaul hash here. out buffer should not be nullptr.
    void querySHA256Verify(SHA256_HANDLE& handle, SHA256_t& out) {
        auto* Module = (Botan::HashFunction*)(handle);
        Module->final(out.u8);
    }
    void querySHA512Verify(SHA512_HANDLE& handle, SHA512_t& out) {
        auto* Module = (Botan::HashFunction*)(handle);
        Module->final(out.u8);
    }

    // deleting, destructing instance.
    // we distruct the instance and recollect into MemPool.
    // 
    // we don't really recommend if you are reusing a object, use flush instead of this.
    void querySHA256Delete(SHA256_HANDLE& handle) {
        auto* Module = (Botan::SHA_3_256*)(handle);
        operator delete(Module, SHA256Module_Pool);
    }
    void querySHA512Delete(SHA512_HANDLE& handle) {
        auto* Module = (Botan::SHA_3_512*)(handle);
        operator delete(Module, SHA512Module_Pool);
    }
    void queryRSADeletePub(RSA_HANDLE& handle) {
    }
    void queryRSADeletePrv(RSA_HANDLE& handle) {
    }

    // flush instance.
    // flush instance values, that stored using verifiy, update function.
    void querySHAFlush(CRYPT_HANDLE& handle) {
        auto* Module = (Botan::HashFunction*)(handle);
        Module->clear();
    }

    // Encrypt, Decrypt functions.
    // basically, we use these functions for signing.
    void queryRSAEncrypt(RSA_HANDLE& handle, void* pOut, void* pIn, size_t szIn) {
    }
    void queryRSADecrypt(RSA_HANDLE& handle, void* pOut, void* pIn, size_t szIn) {
    }
    void queryRSACreateKey(RSA_HANDLE& handle, RSA_t* Prv, RSA_t* Mod) {
    }
}