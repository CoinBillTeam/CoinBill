#ifndef COINBILL_USER_TRANSACTION
#define COINBILL_USER_TRANSACTION

#include <Support/Types.h>

namespace CoinBill
{
    class Wallet;

    enum TransactionType {
        COIN_REWARD         = 0x01, // coin reward type, when you mined a block, proofed a transaction.
        COIN_SEND           = 0x02, // coin send type, when you going to send coins to other account.
    };

    class TransactionHead final {
    public:
        // transaction types are implemented on TransactionType enum.
        // see what kind of type is on.
        TransactionType     m_Type;

        // version of current host.
        uint64_t            m_Version;

        // Time that created transaction.
        // This will be a posix time.
        uint64_t            m_Time;

        // block that this transaction created.
        SHA512_t            m_Block;
    };

    class TransactionBody final {
    public:
    };

    class TransactionNode final
    {
        Wallet*          m_creator;

    protected:
        TransactionHead* m_TransHead;
        TransactionBody* m_TransBody;
        RSA_t            m_TransSign;

        TransactionNode* m_prevNode;
        TransactionNode* m_nextNode;

    public:
        TransactionHead& getHead();
        TransactionBody& getBody();
        TransactionNode& getPrevNode();
        TransactionNode& getNextNode();
        RSA_t& getTransSignature();

        bool isNodeHasSign();
        bool isNodeOwnerSame(Wallet* user);
        bool isNodeCoinTransferType();
        bool isNodeCoinReward();
        bool isNodeSignatureValid();

        bool RefreshNodeData();

        // the sign methods will not work if the wallet isn't have a private key.
        // will fail, returning false.
        bool RefreshNodeSign();
        bool RefreshNodeSign(Wallet* user);
    };

    RSA_t signTransaction(TransactionHead& transaction, Wallet* user);
}

#endif //COINBILL_USER_TRANSACTION
