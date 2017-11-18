#include <User/Host.h>
#include <User/Blockv1.h>
#include <User/Wallet.h>

#include <Support/Logger.h>
#include <Support/Cryption.h>
#include <Support/MemPool.h>

namespace CoinBill
{
    // We use SHA-512 to hash block.
    BlockV1* refreshBlockHash(BlockV1* block, uint32_t cycle) {
        BlockV1*        ret = nullptr;

        SHA512_HANDLE   handle;
        SHA512_t*       hashTmp1 = createSHA512();
        SHA512_t*       hashTmp2 = createSHA512();

        // Zero cycle?.. something is wrong.
        if (cycle == 0)
            return nullptr;

        // create a instance.
        querySHA512Engine(handle);
        try {
            // Pre hashing it before we iterate it.
            querySHAUpdate(handle, &block->m_Header, sizeof(BlockHeaderV1));
            querySHA512Verify(handle, *hashTmp1);

            // To iterate it.
            for (unsigned int i = 0; i < cycle - 1; ++i) {
                querySHAUpdate(handle, *hashTmp1, sizeof(SHA512_t));
                querySHA512Verify(handle, *hashTmp2);

                std::swap(hashTmp1, hashTmp2);
            }

            // copy it.
            block->m_HeaderHash = *hashTmp1;
            ret = block;
        } catch (const std::exception& e) {
            LogErr() << "EXCEPTION WHILE HASHING BLOCK : " << e.what() << std::endl;
        }
        querySHA512Delete(handle);

        // dispose temp object.
        disposeSHA512(hashTmp1);
        disposeSHA512(hashTmp2);
        return ret;
    }

    // Creating a new block based on user, previus block.
    BlockV1* createNewBlock(Wallet* user, BlockV1* prev) {
        bool            newDifficultyTriggered  = false;

        BlockV1*        block                   = new BlockV1();
        BlockHeaderV1&  blockHeader             = block->m_Header;
        
        // We are going to make it more difficult when number is multiple of Host::getNumNewDiff()
        if (prev->m_Header.m_Number % Host::getNumNewDiff())
            newDifficultyTriggered = true;

        // Hash value of previus block.
        blockHeader.m_PrevHash      = prev->m_HeaderHash;

        // Difficulty will changed every 2 month. (just for now)
        // lower value of difficulty is harder. 
        // we are going to make it lower when its triggered.
        blockHeader.m_Difficulty    = prev->m_Header.m_Difficulty - newDifficultyTriggered;
        blockHeader.m_Number        = prev->m_Header.m_Number + 1;

        // Auther of new block. it is Wallet that we are trying to make this block.
        blockHeader.m_Auther        = user->getAccount();
        blockHeader.m_Version       = Host::getHostVersion();
        blockHeader.m_TimeStamp     = Host::getHostTime();
        blockHeader.m_CoinLimit     = 0; // TODO : determine coin limit.
        blockHeader.m_CoinUsed      = 0;
        blockHeader.m_Nonce         = 0;

        // Try refreshing block hash, we updated block header.
        return refreshBlockHash(block, Host::getHashCycle());
    }

    BlockV1* refreshBlockNonce(BlockV1* block, bool rehash) {
        block->m_Header.m_Nonce++;

        // hash if rehash is true
        return rehash ? refreshBlockHash(block, Host::getHashCycle()) : block;
    }

    BlockV1* refreshBlockInfo(BlockV1* block, bool rehash) {
        BlockHeaderV1& blockHeader = block->m_Header;

        // Just refreshing time and versions.
        blockHeader.m_Version       = Host::getHostVersion();
        blockHeader.m_TimeStamp     = Host::getHostTime();

        // hash if rehash is true
        return rehash ? refreshBlockHash(block, Host::getHashCycle()) : block;
    }
}