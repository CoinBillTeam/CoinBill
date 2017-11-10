#ifndef COINBILL_USER_TRANSACTION
#define COINBILL_USER_TRANSACTION

#include <Support/Types.h>

namespace CoinBill
{
    class Wallet;

    enum TransactionType {
        COIN_REWARD,
        COIN_TRANSFER,
    };

    struct TransactionBase
    {
        TransactionType     m_TransType;
        SHA256_t            m_TransAutherSign;
        RSA2048_t           m_TransAuther;
        
        // Time that created transaction.
        // This will be a posix time.
        uint64_t            m_TransTime;
    };

    class TransactionNode 
    {
    protected:
        bool m_isNodeHasSign;

        TransactionBase* m_transaction;
        TransactionNode* m_prevNode;
        TransactionNode* m_nextNode;

    public:
        TransactionBase& getTransaction();
        TransactionNode& getPrevNode();
        TransactionNode& getNextNode();

        RSA2048_t& getTransAuther();
        SHA256_t& getTransAutherSign();

        bool isNodeHasSign();
        bool isNodeOwnerSame(Wallet* user);
        bool isNodeCoinTransferType();
        bool isNodeCoinReward();
        bool isNodeSignatureValid();

        void RefreshNodeData();
        void RefreshNodeSign(Wallet* user);
    };

    SHA256_t signTransaction(TransactionBase& transaction, Wallet* user);
    SHA256_t proofTransaction(TransactionBase& transaction, Wallet* user);
}

#endif
