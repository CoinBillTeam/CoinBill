#include <Support/CryptModule.h>

namespace CoinBill 
{
	void SHA3_256_MODULE::Flush() {
		Engine.clear();
	}
	void SHA3_256_MODULE::Update(void* pBuf, size_t szBuf) {
		Engine.update((uint8_t*)pBuf, szBuf);
	}
	void SHA3_256_MODULE::Verify(SHA256_t * pOut) {
		Engine.final((uint8_t*)pOut);
	}

	void SHA3_512_MODULE::Flush() {
		Engine.clear();
	}
	void SHA3_512_MODULE::Update(void* pBuf, size_t szBuf) {
		Engine.update((uint8_t*)pBuf, szBuf);
	}
	void SHA3_512_MODULE::Verify(SHA512_t * pOut) {
		Engine.final((uint8_t*)pOut);
	}
}