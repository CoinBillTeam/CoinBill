#ifndef COINBILL_SUPPORT_CRYPT_DECL
#define COINBILL_SUPPORT_CRYPT_DECL

#include <Support/BasicType.h>
#include <Support/BasicUtility.h>

#include <botan/sha3.h>
#include <botan/pk_ops.h>
#include <botan/pk_algs.h>
#include <botan/auto_rng.h>
#include <botan/rsa.h>

namespace CoinBill
{
	typedef uint256_t SHA256_t;
	typedef uint512_t SHA512_t;
	typedef uint4096_t RSA_t;

	template <class HashTy>
	class HASH_MODULE {
		virtual void Flush() = 0;
		virtual void Update(void* pBuf, size_t szBuf) = 0;
		virtual void Verify(HashTy* pOut) = 0;
	};

	class SHA3_256_MODULE : public HASH_MODULE<SHA256_t>{
		Botan::SHA_3_256 Engine;

	public:
		void Flush() override;
		void Update(void* pBuf, size_t szBuf) override;
		void Verify(SHA256_t* pOut) override;

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

	class SHA3_512_MODULE : public HASH_MODULE<SHA512_t> {
		Botan::SHA_3_512 Engine;

	public:
		void Flush() override;
		void Update(void* pBuf, size_t szBuf) override;
		void Verify(SHA512_t* pOut) override;

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

	template <class HashTy>
	class RSA_SIGN_MODULE {
		HASH_MODULE<HashTy>*			HashEngine;
		Botan::PK_Ops::Signature*		SignEngine;
		Botan::AutoSeeded_RNG*			RandEngine;

		// We will going to hold a multiple hashs that as we can
		// Because its like kind of wasting by using a single hash per single big signature.
		std::vector<HashTy*> Hashs;

	public:
		RSA_SIGN_MODULE(HASH_MODULE<HashTy>* engine, RSA_t* PrvKey) {
			HashEngine = engine;
			RandEngine = new Botan::AutoSeeded_RNG();
		}
		~RSA_SIGN_MODULE() {
			delete RandEngine;
			delete SignEngine;
		}

		bool InsertHash(HashTy* Hash) {
			if ((Hashs.size() + 1) * sizeof(HashTy) > sizeof(RSA_t) || Hash == nullptr)
				return false;

			Hashs.push_back(Hash);
			return true;
		}

		bool RemoveHash(HashTy* Hash) {
			for (auto Iter = Hashs.begin(); Iter != Hashs.end(); ++Iter)
				if (*Iter == Hash) {
					Hashs.erase(Iter);
					return true;
				}
			return false;
		}

		bool CreateSign(RSA_t* pOutSign) {
			*pOutSign = 0;

			for (auto Hash : Hashs)
				SignEngine->update((uint8_t*)Hash, sizeof(HashTy));

			auto Out = SignEngine->sign(*RandEngine);
			// The value can be overflow.
			// this value is managed in pool as array, so it will be really unsafe.
			// we are going to check the size every time when we are copying.
			if (Out.size() > sizeof(RSA_t))
				return false;

			std::copy(Out.begin(), Out.end(), pOutSign->u8);
			return true;
		}
	};

	template <class HashTy>
	class RSA_VERF_MODULE {
		HASH_MODULE<HashTy>*			HashEngine;
		Botan::PK_Ops::Verification*	VerfEngine;
		Botan::AutoSeeded_RNG*			RandEngine;

		// We will going to hold a multiple hashs that as we can
		// Because its like kind of wasting by using a single hash per single big signature.
		std::vector<HashTy*> Hashs;

	public:
		RSA_VERF_MODULE(HASH_MODULE<HashTy>* engine, RSA_t* PrvKey) {
			HashEngine = engine;
			RandEngine = new Botan::AutoSeeded_RNG();
		}
		~RSA_VERF_MODULE() {
			delete RandEngine;
			delete VerfEngine;
		}

		bool InsertHash(HashTy* Hash) {
			if ((Hashs.size() + 1) * sizeof(HashTy) > sizeof(RSA_t) || Hash == nullptr)
				return false;

			Hashs.push_back(Hash);
			return true;
		}

		bool RemoveHash(HashTy* Hash) {
			for (auto Iter = Hashs.begin(); Iter != Hashs.end(); ++Iter)
				if (*Iter == Hash) {
					Hashs.erase(Iter);
					return true;
				}
			return false;
		}

		void RemoveHash() {
			Hashs.clear();
		}

		bool VerifySign(RSA_t* inSign) {
			for (auto Hash : Hashs)
				VerfEngine->update((uint8_t*)Hash, sizeof(HashTy));
			return VerfEngine->is_valid_signature(inSign, sizeof(RSA_t));
		}
	};
} // end namespace CoinBill

#endif // end define COINBILL_SUPPORT_CRYPT_DECL