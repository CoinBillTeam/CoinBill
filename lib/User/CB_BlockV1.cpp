#include <Support/CryptDecl.h>
#include <Support/CLILogger.h>
#include <User\CB_BlockV1.h>

namespace CoinBill {
    void BlockV1::LazyHashModuleInit() {
        if (HashDeclPri == nullptr)
            HashDeclPri = new SHA512ModuleDecl();
        if (HashDeclSec == nullptr)
            HashDeclSec = new SHA512ModuleDecl();
    }
    void BlockV1::LazySignModuleInit() {
        // SignDecl = new RSAModuleDecl();
    }

    bool BlockV1::updateHash() {
        // We do lazy hash module initialize for saving memory.
        // Initialize when we are going to hash the actual block.
        if (HashDeclPri == nullptr || HashDeclSec == nullptr) {
            // We do not have a hash module.
            // Just creating a new one...
            LazyHashModuleInit();
        }

        // infomation objects are modified.
        // We have to refresh it again.
        if (isInfoObjModified) {
            isInfoObjModified      = false;
            isInfoObjInitalized    = false;

            // Flushing...
            HashDeclPri->Flush();
            HashDeclSec->Flush();
        }

        try {
            // We are checking that we added infomations on module.
            // We do not have to add it again.
            if (!isInfoObjInitalized) {
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
                HashDeclSec->Update(hashBlock);
                
                isInfoObjInitalized = true;
            }
            // Verify hash.
            // We do hash it twice, so we are going to use two hash module.
            // Maybe kind of wasting memories... fix this if we can handle more gently.
            HashDeclPri->Verify(hashBlock);
            HashDeclSec->Verify(hashBlock);

        } catch (std::exception e) {
            // We got something wrong... exit block hashing.
            LogErr() << e.what() << std::endl;
            return false;
        }

        return true;
    }
}