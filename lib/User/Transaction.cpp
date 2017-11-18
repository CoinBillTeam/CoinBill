#include <User/Transaction.h>
#include <User/Host.h>
#include <User/Wallet.h>

#include <Support/Cryption.h>

namespace CoinBill {
    RSA_t signTransaction(TransactionHead& transaction, Wallet* user) {
        RSA_t Sign = 0;

        // You cannot sign without your private key.
        // this wallet is currupted, returning a empty sign.
        if (user->getPrvKey().isEmpty())
            return Sign;

        // Initialize handles.
        CRYPT_HANDLE RSA;
        CRYPT_HANDLE SHA;
        SHA512_t* Hash = createSHA512();

        // Initialize engines.
        querySHA512Engine(SHA);
        queryRSAEnginePrv(RSA, 3, user->getPubKey(), user->getPrvKey());

        // encrypt / sign it.
        querySHAUpdate(SHA, transaction);
        querySHA512Verify(SHA, *Hash);
        queryRSAEncrypt(RSA, Sign, Hash, sizeof(SHA512_t));

        // releasing all objects.
        disposeSHA512(Hash);
        querySHA256Delete(SHA);
        queryRSADelete(RSA);

        // return signed signature.
        return Sign;
    }
    bool verifyTransaction(TransactionHead& transaction, Wallet* user, RSA_t& sign) {
        if (user->getPubKey().isEmpty())
            return false;

        CRYPT_HANDLE RSA;
        CRYPT_HANDLE SHA;
        SHA512_t* Hash1 = createSHA512();
        SHA512_t* Hash2 = createSHA512();

        querySHA512Engine(SHA);
        queryRSAEnginePub(RSA, 3, user->getPubKey());
        
        querySHAUpdate(SHA, transaction);
        querySHA512Verify(SHA, *Hash1);
        queryRSADecrypt(RSA, *Hash2, sign, sizeof(RSA_t));

        bool isEqual = isSHA512HashEqual(*Hash1, *Hash2);

        disposeSHA512(Hash1);
        disposeSHA512(Hash2);
        querySHA256Delete(SHA);
        queryRSADelete(RSA);

        return isEqual;
    }


    TransactionHead& TransactionNode::getHead() {
        // returning the transaction now holding.
        return (*m_TransHead);
    }
    TransactionBody& TransactionNode::getBody() {
        return (*m_TransBody);
    }
    TransactionNode& TransactionNode::getNextNode() {
        // returning next trasaction node.
        return (*m_nextNode);
    }
    TransactionNode& TransactionNode::getPrevNode() {
        // returning prev transaction node.
        return (*m_prevNode);
    }
    RSA_t& TransactionNode::getTransSignature() {
        // returning auther signature of transaction.
        return m_TransSign;
    }

    bool TransactionNode::isNodeHasSign() {
        // Empty sign mean that never initialized transaction sign.
        // we will return false if its empty. not signed.
        return !(getTransSignature().isEmpty());
    }
    bool TransactionNode::isNodeOwnerSame(Wallet* user) {
        // public key value of owner's account should same. 
        // we cannot proof the signature if owner isn't same.
        return (user == m_creator) || (user->getAccount() == m_creator->getAccount());
    }

    bool TransactionNode::isNodeCoinTransferType() {
        return getHead().m_Type == TransactionType::COIN_SEND;
    }

    bool TransactionNode::isNodeCoinReward() {
        return getHead().m_Type == TransactionType::COIN_REWARD;
    }

    bool TransactionNode::isNodeSignatureValid() {
        // Sign doesn't exists.
        if (!isNodeHasSign() || verifyTransaction(getHead(), m_creator, m_TransSign))
            return false;

        return true;
    }

    bool TransactionNode::RefreshNodeData() {
        getHead().m_Version  = Host::getHostVersion();
        getHead().m_Time     = Host::getHostTime();

        return true;
    }

    bool TransactionNode::RefreshNodeSign() {
        return RefreshNodeSign(m_creator);
    }

    bool TransactionNode::RefreshNodeSign(Wallet* user) {
        // Try signing it.
        RSA_t sign = signTransaction(getHead(), user);

        if (sign.isEmpty() || !verifyTransaction(getHead(), user, sign))
            return false;

        // use this sign.
        m_creator           = user;
        m_TransSign         = sign;
        
        return true;
    }
}