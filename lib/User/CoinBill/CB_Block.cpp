#include <Support/CLILogger.h>


#include <User/CoinBill/CB_Hash.h>
#include <User/CoinBill/CB_Block.h>

#include <random>
#include <limits>

namespace CoinBill {
	CB_Block::CB_Block() {
        headBlock.TryCreateAsSecureVH();

		headBlock->nTime            = Host::getHostTime();
		headBlock->nVersion	        = Host::getHostVersion();
		headBlock->nBits            = sizeof(SHA512_t) * 8;
		headBlock->nDifficult       = Host::getLastBlock()->nDifficult;
		headBlock->nCount           = Host::getLastBlock()->nCount + 1;
		headBlock->nNonce           = 0;

		headBlock->hashPrevBlock    = 0;
		headBlock->hashTXRoot       = nullptr; // not implemented yet.
		headBlock->hashAuther       = nullptr; // not implemented yet.

		if (headBlock->nCount % Host::getNumNewDiff() == 0) {
			// TODO : Need algorithm for new difficulty.
		}
	}
    CB_Block::CB_Block(std::string& filename) {

    }

    CB_Block::~CB_Block() {
        if (isHashModuleInitialized) 
            delete hashMoudle;
    }

    void CB_Block::LazyHashModuleInit() {
        hashMoudle = new CB_CommonHash_SHA512();
        isHashModuleInitialized = true;
    }

    bool CB_Block::updateHash() {
        // We do lazy hash module initialize for saving memory.
        // Initialize when we are going to hash the actual block.
        if (isHashModuleInitialized) 
            // We do not have a hash module.
            // Just creating a new one...
            LazyHashModuleInit();

        try {
            hashMoudle->Update(headBlock->nTime);
            hashMoudle->Update(headBlock->nVersion);
            hashMoudle->Update(headBlock->nBits);
            hashMoudle->Update(headBlock->nDifficult);
            hashMoudle->Update(headBlock->nCount);
            hashMoudle->Update(headBlock->nNonce);
            hashMoudle->Update(headBlock->hashPrevBlock);
            hashMoudle->Update(headBlock->hashTXRoot);
            hashMoudle->Update(headBlock->hashAuther);
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
			headBlock->nNonce = device();
        } else {
            // If nonce is maximum.
            // we have to notice we should update time or something else.
            if (headBlock->nNonce++ == std::numeric_limits<decltype(headBlock->nNonce)>::max())
                return false;
        }
        return true;
    }
    void CB_Block::updateTime() {
        // Refresh host time from Host.
        // We will store posix based time.
		headBlock->nTime = Host::getHostTime();
    }
}