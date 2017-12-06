#include <Support/CryptDecl.h>
#include <Support/CryptBase.h>

namespace CoinBill 
{
    // Default constructor.
    // Initializes engine, module, keys.
    RSAModuleDecl::RSAModuleDecl(CRYPT_HANDLE cEngine, RSA_t* Module, RSA_t* Key, bool isInherited) :
        Engine(cEngine)                 ,   // RSA Engine, this will binds RSA functions.
        Mod(Module)                     ,   // RSA Module.
        Prv(Key)                        ,   // RSA Private Key.
        Rnd(createRSA())                ,   // RSA Rounded Hint.
        isSelfInitialized(!isInherited)     // Is self created value? if its false, its inherted.
    { }

    // public key constructor.
    // Use RSA Module as public key handler.
    // The private key is nullptr.
    RSAModuleDecl::RSAModuleDecl(CRYPT_HANDLE cEngine, RSA_t* Module) : 
        RSAModuleDecl(cEngine, Module, nullptr, true) 
    { }

    // private key constructor
    // Use RSA Module as private key handler.
    RSAModuleDecl::RSAModuleDecl(CRYPT_HANDLE cEngine, RSA_t* Module, RSA_t* Key) :
        RSAModuleDecl(cEngine, Module, Key, true) 
    { }

    RSAModuleDecl::~RSAModuleDecl() {
        // We will dispose object if its self initialized.
        if (isSelfInitialized) {
            // Dispose RSA Object.
            disposeRSA(Mod);

            if (Prv) {
                disposeRSA(Prv);
                queryRSADeletePrv(Engine);
            } else {
                queryRSADeletePub(Engine);
            }
        }
        disposeRSA(Rnd);
    }

    RSAModuleDecl::RSAModuleDecl() : 
        Mod(createRSA()),
        Prv(createRSA()),
        Rnd(createRSA())
    {
        queryRSAEnginePrv(Engine, 3, *Prv, *Mod);
    }

    bool RSAModuleDecl::isInitialized() const {
        return Mod == nullptr;
    }

    // We are going to check it that is private encryption.
    // returning false when its public encryption.
    bool RSAModuleDecl::hasPrvRoundHint() {
        return isPrivateEncryption == true ?
            (Rnd != nullptr) : (false);
    }
    bool RSAModuleDecl::hasPubRoundHint() {
        return isPrivateEncryption != true ?
            (Rnd != nullptr) : (false);
    }

    // TODO:
    //  The rounded key isn't valid yet.
    //  we are going to implement it later.
    bool RSAModuleDecl::tryCreatePrvRoundHint() {
        return true;
    }
    void RSAModuleDecl::removePrvRoundHint() { Rnd = 0; }
    bool RSAModuleDecl::tryCreatePubRoundHint() { 
        return true; 
    }
    void RSAModuleDecl::removePubRoundHint() { Rnd = 0; }


    // TODO :
    //  Key check isn't implemented yet.
    bool RSAModuleDecl::isModuleValid() {
        return true;
    }
    bool RSAModuleDecl::isModuleSafe() {
        return true;
    }
    bool RSAModuleDecl::isModuleSecure() {
        return true;
    }

    void RSAModuleDecl::InitNewKey() {
        queryRSACreateKey(Engine, Prv, Mod);
    }

    bool SignatureModule::tryCreateSignature(SHA256_t* Hash, RSA_t* Sign) {
        queryRSAEncrypt(Engine, Sign, Hash, sizeof(SHA256_t));
        return true;
    }
    bool SignatureModule::tryCreateSignature(SHA512_t* Hash, RSA_t* Sign) {
        queryRSAEncrypt(Engine, Sign, Hash, sizeof(SHA512_t));
        return true;
    }
    bool SignatureModule::checkSignAvailable(SHA256_t* Hash, RSA_t* Sign) {
        SHA256_t tempHash = 0;
        queryRSADecrypt(Engine, &tempHash, Sign, sizeof(SHA256_t));
        return isSHA256HashEqual(tempHash, *Hash);
    }
    bool SignatureModule::checkSignAvailable(SHA512_t* Hash, RSA_t* Sign) {
        SHA512_t tempHash = 0;
        queryRSADecrypt(Engine, &tempHash, Sign, sizeof(SHA512_t));
        return isSHA512HashEqual(tempHash, *Hash);
    }

    SHA256ModuleDecl::SHA256ModuleDecl() {
        querySHA256Engine(Engine);
    }
    SHA256ModuleDecl::~SHA256ModuleDecl() {
        querySHADelete(Engine);
    }
    void SHA256ModuleDecl::Flush() {
        querySHAFlush(Engine);
    }
    void SHA256ModuleDecl::Verify(SHA256_t * pOut) {
        querySHA256Verify(Engine, *pOut);
    }

    SHA512ModuleDecl::SHA512ModuleDecl() {
        querySHA512Engine(Engine);
    }
    SHA512ModuleDecl::~SHA512ModuleDecl() {
        querySHADelete(Engine);
    }
    void SHA512ModuleDecl::Flush() {
        querySHAFlush(Engine);
    }
    void SHA512ModuleDecl::Verify(SHA512_t * pOut) {
        querySHA512Verify(Engine, *pOut);
    }
}