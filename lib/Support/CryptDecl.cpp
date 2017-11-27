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
            disposeRSA(Prv);

            // Dispose RSA Engine.
            queryRSADelete(Engine);
        }
        disposeRSA(Rnd);
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
    bool RSAModuleDecl::isPrivateKeyValid() {
        return true;
    }
    bool RSAModuleDecl::isPrivateKeySafe() {
        return true;
    }
    bool RSAModuleDecl::isPrivateKeySecure() {
        return true;
    }
}