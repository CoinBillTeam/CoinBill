#ifndef COINBILL_USER_WALLET
#define COINBILL_USER_WALLET

#include <vector>
#include <Support/Types.h>
#include <Support/Cryption.h>

namespace CoinBill
{
    class BlockV1;

    class WalletData
    {
    public:
        
        RSA_t   m_PubKey;
        RSA_t   m_PrvKey;

        SHA512_t    m_Block;
        uint64_t    m_Money;
    };

    class Wallet
    {
        SHA256_t    m_account;
        WalletData  m_data;

    public:
        SHA256_t&   getAccount();
        RSA_t&      getPubKey();
        RSA_t&      getPrvKey();
        
        SHA512_t&   getWalletBlock();
        uint64_t    getWalletCoin();
        WalletData& getWalletData();

        bool refreshAccount();
        bool refreshPublicKey();
        void dump();
    };

    Wallet* createManagedWallet();
    void deleteManagedWallet(CoinBill::Wallet* wallet);

    Wallet* createAccount(BlockV1* block, RSA_t& PrvKey, RSA_t& PubKey);
    Wallet* createAccount(BlockV1* block, RSA_t& PrvKey);
    Wallet* createAccountPrv(RSA_t& PrvKey);
    Wallet* createAccountPub(RSA_t& PubKey);
    Wallet* createAccount();
}

#endif // COINBILL_USER_WALLET