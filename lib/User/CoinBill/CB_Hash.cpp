#include <User\CoinBill\CB_Hash.h>

namespace CoinBill {
	// Our common hash performes hash twice.

	void CB_CommonHash_SHA256::Verify(SHA256_t* pOut) {
		SHA256_t tmpHash;

		SHA3_256_MODULE::Verify(&tmpHash);
		SHA3_256_MODULE::Update(&tmpHash, sizeof(SHA256_t));
		SHA3_256_MODULE::Verify(pOut);
	}

	void CB_CommonHash_SHA512::Verify(SHA512_t* pOut) {
		SHA512_t tmpHash;

		SHA3_512_MODULE::Verify(&tmpHash);
		SHA3_512_MODULE::Update(&tmpHash, sizeof(SHA512_t));
		SHA3_512_MODULE::Verify(pOut);
	}
}