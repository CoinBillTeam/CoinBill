#ifndef COINBILL_USER_HOST
#define COINBILL_USER_HOST

#include <cstdint>
#include <vector>

#include <Support/BasicType.h>
#include <Support/ResourcePool.h>

namespace CoinBill
{
    class BlockV1;
    class Wallet;
    class Socket;

    class Host
    {
    public:
        // TODO : implemnt notify methods based on P2P network.
        //        so that we can send / share objects on other peers.
        void notifyNewContract();
        void notifyNewAccount();
        void notifyNewTransaction();
        void notifyConnectedPeers();
        void ProcessQueues();

        static uint64_t getHostVersion();
        static uint64_t getHostTime();
        static uint64_t getNumNewDiff();
        static uint32_t getHashCycle();
        static BlockV1* getLastBlock();

    private:
        Wallet* hostWallet;
        Socket* hostSocket;
    };


    Host* createHost(Wallet* wallet);
}

#endif // COINBILL_USER_HOST