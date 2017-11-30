#ifndef COINBILL_USER_CB_BLOCK_V1
#define COINBILL_USER_CB_BLOCK_V1

#include <Support/CryptType.h>
#include <Support/CryptDecl.h>

namespace CoinBill {

    struct BlockHeadV1 {
        uint64_t nTime;
        uint32_t nChannel;
        uint32_t nVersion;
        uint32_t nBits;
        uint64_t nDifficult;
        uint64_t nCount;
        uint64_t nNonce;

        SHA512_t* hashPrevBlock;
        SHA256_t* hashTXRoot;
    };

    class BlockV1 : public BlockHeadV1 {
        SHA512_t* hashBlock;
        bool isInfoObjModified;
        bool isInfoObjInitalized;

        SHA512ModuleDecl* HashDeclPri;
        SHA512ModuleDecl* HashDeclSec;
        RSAModuleDecl* SignDecl;

        void LazyHashModuleInit();
        void LazySignModuleInit();

    public:
        BlockV1();
        virtual ~BlockV1();


        bool updateHash();
        void updateNonce(bool isRandom);
        void updateTime();
    };


    class BlockTXV1 : public BlockHeadV1 {

    };
}

#endif