#ifndef COINBILL_USER_CB_BLOCK_V1
#define COINBILL_USER_CB_BLOCK_V1

#include <Support/BasicUtility.h>
#include <Support/CryptType.h>
#include <Support/CryptDecl.h>

namespace CoinBill {
    class CB_Wallet;
    struct CB_BlockHeadV1 {
        uint64_t        nTime;
        uint32_t        nChannel;
        uint32_t        nVersion;
        uint32_t        nBits;
        uint64_t        nDifficult;
        uint64_t        nCount;
        uint64_t        nNonce;

        SHA512_t*       hashPrevBlock;
        SHA256_t*       hashTXRoot;
        SHA256_t*       hashAuther;
    };

    class NOVTABLE CB_BlockV1 : public CB_BlockHeadV1 {
        SHA512_t* hashBlock;
        bool isInfoObjModified;
        bool isInfoInitalized;

        bool isHashModuleInitialized;
        bool isHashObjectInitialized;

        // Primary hash module for hashing block head.
        SHA512ModuleDecl* HashDeclPri;

        // Secondary hash module for hashing previous hash.
        // So we will hash twice.
        SHA512ModuleDecl* HashDeclSec;

        // User that who created this block.
        // This will be auther of block.
        CB_Wallet* User;

    protected:
        // Lazy object initializers.
        // We allocate objects when it is going to use.
        void LazyHashModuleInit();
        void LazyHashObjectInit();

    public:
        CB_BlockV1();
        virtual ~CB_BlockV1();

        // update current hash of block.
        // The hash object will lazily created when updateHash is called.
        // same for hash modules.
        bool updateHash();

        // Updating nonce, you will need to update time or something else if its failed.
        // the falied mean nonce value is maximum.
        bool updateNonce(bool isRandom = false);
        void updateTime();
    };


    // This block is half-difficult for extra transaction that cannot be included to original block.
    // this can only created by who created lastest block. 
    // if the block created, you get half reward of your previous block. (because half-difficulty)
    // Only triggered when current collected TX-Pool has over 1.3k transactions.
    class NOVTABLE CB_BlockTxV1 : public CB_BlockV1 {
        unsigned int transactions;

    public:
        // Not implemented yet!
    };

    CB_BlockV1* createBlock();
    CB_BlockV1* createBlockWithJSON(const std::string& file);
}

#endif