#ifndef COINBILL_USER_CB_BLOCK_V1
#define COINBILL_USER_CB_BLOCK_V1

#include <Support/BasicUtility.h>
#include <Support/CryptModule.h>

namespace CoinBill {
    class CB_Wallet;
	class CB_CommonHash_SHA512;

    struct CB_BlockHead {
        uint64_t nTime;
        uint32_t nVersion;
        uint32_t nBits;
        uint64_t nDifficult;
        uint64_t nCount;
        uint64_t nNonce;

        SHA512_t* hashPrevBlock;
        SHA256_t* hashTXRoot;
        SHA256_t* hashAuther;
    };

    class NOVTABLE CB_Block : public CB_BlockHead {
        SHA512_t* hashBlock;

        bool isHashModuleInitialized;
        bool isHashObjectInitialized;

        // Primary hash module for hashing block head.
		CB_CommonHash_SHA512* hashMoudle;

        // User that who created this block.
        // This will be auther of block.
        CB_Wallet* User;

        // Transaction infomations;
        unsigned int transactions;

    protected:
        // Lazy object initializers.
        // We allocate objects when it is going to use.
        void LazyHashModuleInit();
        void LazyHashObjectInit();

    public:
        CB_Block();
        virtual ~CB_Block();

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
    // if the block created, you get half reward of your previous block with transaction fee (because half-difficulty)
    // Only triggered when current collected TX-Pool has over 1.3k transactions.
    // The block will not accepted if this block doesn't includes over 1k transactions.
    class NOVTABLE CB_BlockTxV1 : public CB_Block {

    public:
        // Not implemented yet!
    };

    CB_Block* createBlock();
    CB_Block* createBlockWithJSON(const std::string& file);
}

#endif