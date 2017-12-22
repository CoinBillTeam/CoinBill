#ifndef COINBILL_USER_CB_WALLET
#define COINBILL_USER_CB_WALLET

#include <Support/CryptModule.h>
#include <string>

namespace CoinBill {
    class CB_Wallet final {
        SHA256_t* hashAccount256;
        SHA512_t* hashAccount512;

        RSA_t* Account;
        RSA_t* Key;

        SHA256_t* LazyHashInit256();
        SHA512_t* LazyHashInit512();

    public:
        CB_Wallet(RSA_t* account, RSA_t* key);
        ~CB_Wallet();

        RSA_t* getAccount();
        RSA_t* getAccountKey();
        SHA256_t* getAccountHash256();
        SHA512_t* getAccountHash512();
    };

    CB_Wallet* createCBWallet();
    CB_Wallet* createCBWalletFromJSON(const std::string& filename);
}

#endif // end define COINBILL_USER_CB_WALLET