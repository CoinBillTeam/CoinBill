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
		virtual void Flush() override;
		virtual void Update(void* pBuf, size_t szBuf) override;
		virtual void Verify(SHA256_t* pOut) override;

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
		virtual void Flush() override;
		virtual void Update(void* pBuf, size_t szBuf) override;
		virtual void Verify(SHA512_t* pOut) override;

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

		void InsertHash(HashTy* Hash) {
			if(Hash != nullptr) 
				Hashs.push_back(Hash);
		}

		void RemoveHash(HashTy* Hash) {
			for (auto Iter = Hashs.begin(); Iter != Hashs.end(); ++Iter)
				if (*Iter == Hash)
					Hashs.erase(Iter);
		}

		size_t CreateSign(RSA_t* pOutSign) {
			for (unsigned int i = 0; i < Hashs.size(); ++i)
				SignEngine->update((uint8_t*)Hashs[i], sizeof(HashTy));

			auto Out = SignEngine->sign(*RandEngine);
			for (unsigned int i = 0; i != Out.size(); ++i) {
				// The value can be overflow.
				// this value is managed in pool as array, so it will be really unsafe.
				// we are going to check the size every time when we are copying.
				if (i >= sizeof(RSA_t))
					return Out.size() - sizeof(RSA_t);
				((uint8_t*)pOutSign)[i] = Out[i];
			}
			return 0;
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

		void InsertHash(HashTy* Hash) {
			if (Hash != nullptr)
				Hashs.push_back(Hash);
		}

		void RemoveHash(HashTy* Hash) {
			for (auto Iter = Hashs.begin(); Iter != Hashs.end(); ++Iter)
				if (*Iter == Hash)
					Hashs.erase(Iter);
		}

		void RemoveHash() {
			Hashs.clear();
		}

		bool VerifySign(RSA_t* inSign) {
			for (unsigned int i = 0; i < Hashs.size(); ++i)
				VerfEngine->update((uint8_t*)Hashs[i], sizeof(HashTy));
			return VerfEngine->is_valid_signature(inSign, sizeof(RSA_t));
		}
	};
} // end namespace CoinBill

#endif // end define COINBILL_SUPPORT_CRYPT_DECL