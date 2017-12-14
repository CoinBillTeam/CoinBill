#include <User/Host.h>
#include <User/CoinBill/CB_Block.h>

#include <ctime>

CoinBill::SHA256_t Zero256V = 0;
CoinBill::SHA512_t Zero512V = 0;
CoinBill::CB_BlockHead GenesisBlock = {
    1512021486              , // Time
    0                       , // Channel
    1                       , // Version
    512                     , // Bits
    1                       , // Difficult
    0                       , // Count
    0                       , // Nonce
    &Zero512V               , // Prev Block.
    &Zero256V               , // TX Root.
    &Zero256V                 // Auther
};

namespace CoinBill
{
    // TODO_ALL : need to implements based on platform.
    //            we do only support MSVC for now.
    uint64_t Host::getHostVersion() {
        // TODO : CMake based versioning.
        return 1;
    }
    uint64_t Host::getHostTime() {
        // TODO : Use posix based time.
        time_t t;
        time(&t);

        return (uint64_t)t;
    }
    uint64_t Host::getNumNewDiff() {
        // We generate new difficulty every 1 hour.
        return 2 * 60;
    }
    CB_BlockHead* Host::getLastBlock() {
        return &GenesisBlock;
    }
}