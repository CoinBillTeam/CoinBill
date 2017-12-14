#ifndef COINBILL_SUPPORT_CRYPT_DECL
#define COINBILL_SUPPORT_CRYPT_DECL

#include <Support/BasicType.h>
#include <Support/BasicUtility.h>

#include <botan/sha3.h>

namespace CoinBill
{
	typedef uint256_t SHA256_t;
	typedef uint512_t SHA512_t;
	typedef uint4096_t RSA_t;

	template <class OutTy>
	class NOVTABLE HashModule {
	public:
		virtual void Flush() = 0;
		virtual void Update(void* pBuf, size_t szBuf) = 0;
		virtual void Verify(OutTy* pOut) = 0;
	};

	class SHA3_256_MODULE : public HashModule<SHA256_t> {
		Botan::SHA_3_256 Engine;

	public:
		void Flush();
		void Update(void* pBuf, size_t szBuf);
		void Verify(SHA256_t* pOut);

		template <class Ty, size_t size>
		void Update(Ty(&pBuf)[size]) {
			Update((void*)&pBuf, sizeof(Ty) * size);
		}
		template <class Ty>
		void Update(Ty* pBuf, size_t size = 1) {
			Update((void*)pBuf, sizeof(Ty) * size);
		}
		template <class Ty>
		void Update(Ty& pBuf) {
			Update((void*)&pBuf, sizeof(Ty));
		}
	};

	class SHA3_512_MODULE : public HashModule<SHA512_t> {
		Botan::SHA_3_512 Engine;

	public:
		void Flush();
		void Update(void* pBuf, size_t szBuf);
		void Verify(SHA512_t* pOut);

		template <class Ty, size_t size>
		void Update(Ty(&pBuf)[size]) {
			Update((void*)&pBuf, sizeof(Ty) * size);
		}
		template <class Ty>
		void Update(Ty* pBuf, size_t size = 1) {
			Update((void*)pBuf, sizeof(Ty) * size);
		}
		template <class Ty>
		void Update(Ty& pBuf) {
			Update((void*)&pBuf, sizeof(Ty));
		}
	};

} // end namespace CoinBill

#endif // end define COINBILL_SUPPORT_CRYPT_DECL