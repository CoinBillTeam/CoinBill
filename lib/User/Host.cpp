#include <User/Host.h>

#include <time.h>

namespace CoinBill
{
    // TODO_ALL : need to implements based on platform.
    //            we do only support MSVC for now.
    uint64_t Host::getHostVersion() {
        // TODO : CMake based versioning.
        return 0;
    }
    uint64_t Host::getHostTime() {
        // TODO : Use posix based time.
        time_t t;
        time(&t);

        return (uint64_t)t;
    }
    uint64_t Host::getNumNewDiff() {
        // We generate new difficulty every 2 month.
        return 2 * 60 * 30 * 2;
    }
    uint32_t Host::getHashCycle() {
        // The default hash cycle for block is 2.
        return 2;
    }
    BlockV1* Host::getLastBlock() {
        return nullptr;
    }


    // Host Implements.
}