#include <Support/CLILogger.h>
#include <Support/ResourceVH.h>

#include <User/CoinBill/CB_Hash.h>
#include <User/CoinBill/CB_Block.h>

#include <random>
#include <limits>

namespace CoinBill {
    CB_Block::CB_Block() {
        nTime           = Host::getHostTime();
        nVersion        = Host::getHostVersion();
        nBits           = sizeof(SHA512_t) * 8;
        nDifficult      = Host::getLastBlock()->nDifficult;
        nCount          = Host::getLastBlock()->nCount + 1;
        nNonce          = 0;

        hashPrevBlock   = 0;
        hashTXRoot      = nullptr; // not implemented yet.
        hashAuther      = nullptr; // not implemented yet.

        if (nCount % Host::getNumNewDiff() == 0) {
            // TODO : Need algorithm for new difficulty.
        }
    }
    CB_Block::~CB_Block() {
		if (isHashObjectInitialized)
			delete hashBlock;
        if (isHashModuleInitialized) 
            delete hashMoudle;
    }

    void CB_Block::LazyHashModuleInit() {
        hashMoudle = new CB_CommonHash_SHA512();
        isHashModuleInitialized = true;
    }

    void CB_Block::LazyHashObjectInit() {
        hashBlock = new SHA512_t();
        isHashObjectInitialized = true;
    }

    bool CB_Block::updateHash() {
        // We do lazy hash module initialize for saving memory.
        // Initialize when we are going to hash the actual block.
        if (isHashModuleInitialized) 
            // We do not have a hash module.
            // Just creating a new one...
            LazyHashModuleInit();

        // Same here, we are only initialize as that we need.
        if (isHashObjectInitialized) 
            // Create a new hash.
            LazyHashObjectInit();

        try {
            hashMoudle->Update(nTime);
            hashMoudle->Update(nVersion);
            hashMoudle->Update(nBits);
            hashMoudle->Update(nDifficult);
            hashMoudle->Update(nCount);
            hashMoudle->Update(nNonce);
            hashMoudle->Update(hashPrevBlock);
            hashMoudle->Update(hashTXRoot);
            hashMoudle->Update(hashAuther);
            hashMoudle->Verify(hashBlock);

        } catch (const std::exception& e) {
            // We got something wrong... exit block hashing.
            LogErr() << e.what() << std::endl;
            return false;
        }
        return true;
    }
    bool CB_Block::updateNonce(bool isRandom) {
        if (isRandom) {
            std::random_device device;
            nNonce = device();
        } else {
            // If nonce is maximum.
            // we have to notice we should update time or something else.
            if (nNonce++ == std::numeric_limits<decltype(nNonce)>::max())
                return false;
        }
        return true;
    }
    void CB_Block::updateTime() {
        // Refresh host time from Host.
        // We will store posix based time.
        nTime = Host::getHostTime();
    }
}