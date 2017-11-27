#ifndef COINBILL_SUPPORT_CRYPT_DECL
#define COINBILL_SUPPORT_CRYPT_DECL

#include <Support/CryptType.h>
#include <Support/CryptBase.h>

namespace CoinBill
{
    class SHAModuleDecl {
        // Hash function binding engine
        // the sha hash function will created when you initializing.
        CRYPT_HANDLE Engine;
    };

    // RSA Module Declares, this will handle RSA cryption globally.
    // Do not create RSAModuleDecl directly, we do handle by inherting it.
    class RSAModuleDecl {
        bool isSelfInitialized      = true;
        bool isPrivateEncryption    = true;

        // Default constructor.
        explicit RSAModuleDecl(CRYPT_HANDLE cEngine, RSA_t* Module, RSA_t* Key, bool isInherited = false);

    protected:
        // The RSA objects will automaically managed by MemPool.
        // Do not release it manually. The objects will deposed when its destructing.

        RSA_t* Mod; // RSA Module.
        RSA_t* Prv; // RSA Private Key. (can nullptr)
        RSA_t* Rnd; // RSA Round Hint. (can nullptr)
        const short PubExp = 3;

        // Crypt function binding engine
        // if you created the module declare as signing.
        // the object will be initialized for signing.
        // or the object will be initialized fro crypting. public / private key pair.
        CRYPT_HANDLE Engine;
        
        // we only can initialized by inherted classes.
        explicit RSAModuleDecl(CRYPT_HANDLE cEngine, RSA_t* Module);
        explicit RSAModuleDecl(CRYPT_HANDLE cEngine, RSA_t* Module, RSA_t* Key);

        // The destructure will dispose everything that we used on MemPool.
        virtual ~RSAModuleDecl();
    public:
        // checking that this module declare has private rounded hint.
        // the rounded hint will help cryption job much faster.
        // this is really useful when you are going to decrypt with private as multiple times.
        bool hasPrvRoundHint();
        bool hasPubRoundHint();

        // creating / removing rounded hint.
        // This is really useful when you are going to decrypt with private as multiple time.
        // creating rounded hint will take much longer than reusing rounded hint.
        // but be careful all of objects can be decrypted if someone has the rounded hint.
        bool tryCreatePrvRoundHint();
        void removePrvRoundHint();
        bool tryCreatePubRoundHint();
        void removePubRoundHint();

        // The key check methods.
        // isPrivateKeyValid will check minimum require for RSA key. but fastest.
        // isPrivateKeySafe will check its safe to use.
        // isPrivateKeySecure will check its really secure to use. but slowest.
        // recommend isPrivateKeySafe or isPrivateKeySecure when you intiailizing key.
        // use isPrivateKeyValid when you just checking the key valid.
        bool isPrivateKeyValid();
        bool isPrivateKeySafe();
        bool isPrivateKeySecure();
    };

    template <class HashType, class HashModule>
    class SignatureModule : public RSAModuleDecl {
    public:
        // Try to create signature with hashed value.
        bool tryCreateSignature(HashType* Hash, RSA_t* Sign);

        // Check created sign available and valid.
        bool checkSignAvailable(HashType* Hash, RSA_t* Sign);
    };
} // end namespace CoinBill

#endif // end define COINBILL_SUPPORT_CRYPT_DECL