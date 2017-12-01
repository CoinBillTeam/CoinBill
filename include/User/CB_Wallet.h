#ifndef COINBILL_USER_CB_WALLET
#define COINBILL_USER_CB_WALLET

#include <Support/CryptType.h>
#include <Support/CryptDecl.h>

namespace CoinBill {
    class CB_Wallet {
        double nBalance;

    public:
        RSA_t* Module;
        RSA_t* Key;

        SignatureModule SignEngine;
    };

    CB_Wallet* createCBWallet();
    CB_Wallet* createCBWalletFromJSON(const std::string& filename);
}

#endif // end define COINBILL_USER_CB_WALLET