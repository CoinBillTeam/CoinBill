#ifndef COINBILL_USER_CB_WALLET
#define COINBILL_USER_CB_WALLET

#include <Support/CryptModule.h>

namespace CoinBill {
    class CB_Wallet {
        SHA256_t* hashMod;

    public:
        RSA_t* Mod;
        RSA_t* Key;
    };

    CB_Wallet* createCBWallet();
    CB_Wallet* createCBWalletFromJSON(const std::string& filename);
}

#endif // end define COINBILL_USER_CB_WALLET