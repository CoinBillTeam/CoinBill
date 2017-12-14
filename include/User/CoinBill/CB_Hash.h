#ifndef COINBILL_USER_HASH
#define COINBILL_USER_HASH

#include <Support/CryptModule.h>

namespace CoinBill {
	class CB_CommonHash_SHA256 : public SHA3_256_MODULE{
	public:
		void Verify(SHA256_t* pOut);
    };

	class CB_CommonHash_SHA512 : public SHA3_512_MODULE {
	public:
		void Verify(SHA512_t* pOut);
	};
}

#endif