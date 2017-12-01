#include <Support/CryptDecl.h>
#include <Support/CLILogger.h>
#include <User/CB_BlockV1.h>

#include <random>
#include <limits>

namespace CoinBill {
    CB_BlockV1::CB_BlockV1() {
        nTime           = Host::getHostTime();
        nChannel        = 0;
        nVersion        = Host::getHostVersion();
        nBits           = sizeof(SHA512_t) * 8;
        nDifficult      = Host::getLastBlock()->nDifficult;
        nCount          = Host::getLastBlock()->nCount + 1;
        nNonce          = 0;

        hashPrevBlock   = 0;
        hashTXRoot      = nullptr; // not implemented yet.
        hashAuther      = nullptr; // not implemented yet.

        if (nCount % Host::getNumNewDiff()) {
            // TODO : Need algorithm for new difficulty.
        }
    }
    CB_BlockV1::~CB_BlockV1() {
        if (isHashObjectInitialized) {
            disposeSHA512(hashBlock);
        }
        if (isHashModuleInitialized) {
            delete HashDeclPri;
            delete HashDeclSec;
        }
    }

    void CB_BlockV1::LazyHashModuleInit() {
        HashDeclPri = new SHA512ModuleDecl();
        HashDeclSec = new SHA512ModuleDecl();
        isHashModuleInitialized = true;
    }

    void CB_BlockV1::LazyHashObjectInit() {
        hashBlock = createSHA512();
        isHashObjectInitialized = true;
    }

    bool CB_BlockV1::updateHash() {
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

        // infomation objects are modified.
        // We have to refresh it again.
        if (isInfoObjModified) {
            isInfoObjModified       = false;
            isInfoInitalized        = false;

            // Flushing...
            HashDeclPri->Flush();
            HashDeclSec->Flush();
        }

        try {
            // We are checking that we added infomations on module.
            // We do not have to add it again.
            if (!isInfoInitalized) {
                // We checked that infomation didn't initialized.
                // Adding infomation to Hash module.
                // We do not have to set update again.
                HashDeclPri->Update(nTime);
                HashDeclPri->Update(nChannel);
                HashDeclPri->Update(nVersion);
                HashDeclPri->Update(nBits);
                HashDeclPri->Update(nDifficult);
                HashDeclPri->Update(nCount);
                HashDeclPri->Update(nNonce);
                HashDeclPri->Update(hashPrevBlock);
                HashDeclPri->Update(hashTXRoot);
                HashDeclPri->Update(hashAuther);
                HashDeclSec->Update(hashBlock);
                
                isInfoInitalized = true;
            }
            // Verify hash.
            // We do hash it twice, so we are going to use two hash module.
            // Maybe kind of wasting memories... fix this if we can handle more gently.
            HashDeclPri->Verify(hashBlock);
            HashDeclSec->Verify(hashBlock);

        } catch (const std::exception& e) {
            // We got something wrong... exit block hashing.
            LogErr() << e.what() << std::endl;
            return false;
        }
        return true;
    }
    bool CB_BlockV1::updateNonce(bool isRandom) {
        if (isRandom) {
            std::random_device device;
            nNonce = device();
        } else {
            // If nonce is maximum.
            if (nNonce++ == std::numeric_limits<decltype(nNonce)>::max())
                return false;
        }
        return true;
    }
    void CB_BlockV1::updateTime() {
        // Refresh host time from Host.
        // We will store posix based time.
        nTime = Host::getHostTime();
    }
}