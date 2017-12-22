#include <User/CoinBill/CB_Hash.h>
#include <User/CoinBill/CB_Wallet.h>

#include <mutex>

std::mutex 						globalHash256Mutex;
CoinBill::CB_CommonHash_SHA256 	globalHash256;

std::mutex 						globalHash512Mutex;
CoinBill::CB_CommonHash_SHA512  globalHash512;

namespace CoinBill {
	CB_Wallet::CB_Wallet(RSA_t* account, RSA_t* key) : Account(account), Key(key) { }
	CB_Wallet::~CB_Wallet() {
		if(hashAccount256 != nullptr) delete hashAccount256;
		if(hashAccount512 != nullptr) delete hashAccount512;
	}

	SHA256_t* CB_Wallet::LazyHashInit256() {
		hashAccount256 = new SHA256_t();

		{
			std::lock_guard<std::mutex> lg(globalHash256Mutex);
			globalHash256.Update(Account);
			globalHash256.Verify(hashAccount256);
		}

		return hashAccount256;
	}

	SHA512_t* CB_Wallet::LazyHashInit512() {
		hashAccount512 = new SHA512_t();

		{
			std::lock_guard<std::mutex> lg(globalHash512Mutex);
			globalHash512.Update(Account);
			globalHash512.Verify(hashAccount512);
		}

		return hashAccount512;
	}

	RSA_t* CB_Wallet::getAccount() {
		return Account;
	}
	RSA_t* CB_Wallet::getAccountKey() {
		return Key;
	}
	SHA256_t* CB_Wallet::getAccountHash256() {
		return hashAccount256 != nullptr ? hashAccount256 : LazyHashInit256();
	}
	SHA512_t* CB_Wallet::getAccountHash512() {
		return hashAccount512 != nullptr ? hashAccount512 : LazyHashInit512();
	}
}