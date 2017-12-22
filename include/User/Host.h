#ifndef COINBILL_USER_HOST
#define COINBILL_USER_HOST

#include <cstdint>
#include <vector>

#include <Support/BasicType.h>
#include <Support/ResourcePool.h>
#include <User/CoinBill/CB_Block.h>

namespace CoinBill
{
    class CB_Block;
    class CB_Wallet;
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

        static uint32_t getHostVersion();
        static uint64_t getHostTime();
        static uint64_t getNumNewDiff();
        static CB_BlockHead* getLastBlock();

    private:
        CB_Wallet* hostWallet;
        Socket* hostSocket;
    };


    // Host* createHost(Wallet* wallet);
}

#endif // COINBILL_USER_HOST