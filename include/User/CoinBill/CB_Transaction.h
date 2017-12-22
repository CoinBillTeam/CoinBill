#ifndef COINBILL_USER_TRANSACTION_V1
#define COINBILL_USER_TRANSACTION_V1

#include <vector>
#include <string>
#include <Support/CryptModule.h>

namespace CoinBill {
    enum class CB_TxType {
		ACCOUNT_REGISTER							= 0x1001,
        ACCOUNT_SEND_BALANCE						= 0x1002,
		ACCOUNT_SET_INFO							= 0x1003,
		ACCOUNT_SET_SHORT_NAME						= 0x1004,
		ACCOUNT_NODIFY_WARNING						= 0x1005,
		ACCOUNT_BAN									= 0x1006,
		ACCOUNT_UNBAN								= 0x1007,
		ACCOUNT_AGREE_ON_ADMIN						= 0x1008,

		TRANSACTION_BLOCK_PROOF						= 0x1011,
		TRANSACTION_PROOF							= 0x1012,
		TRANSACTION_REQ_DISCARD						= 0x1013,

		CONTRACT_CREATE								= 0x1021,
		CONTRACT_DISCARD							= 0x1022,
		CONTRACT_GIVE_OWNSHIP						= 0x1023,
		CONTRACT_DO_EXECUTE							= 0x1024,

		BLOCK_CREATION_NODIFY_POW					= 0x10AE,
		BLOCK_CREATION_NODIFY_POS					= 0x10AF,

		// Remember that admin has no permission, just asking nodes to accept it.
		// This transactions will not applied if the nodes refused.
		ADMIN_AGREE_AND_ACCEPT						= 0x10D0,
		ADMIN_CREATE_NEW							= 0x10D1,
		ADMIN_ACCOUNT_BAN							= 0x10F0,
		ADMIN_ACCOUNT_UNBAN							= 0x10F1,
		ADMIN_GIVE_COIN								= 0x10F2,
		ADMIN_TAKE_COIN								= 0x10F3,
		ADMIN_SET_BLOCK_POS							= 0x10F4,
		ADMIN_SET_NEXT_BLOCK						= 0x10F5,
		ADMIN_STOP_TRANSACTIONS						= 0x10F6,
		ADMIN_STOP_BLOCK_CREATION					= 0x10F7
    };

	class CB_TxBI {
	public:
		CB_TxType TxType;

		uint64_t nVersion;
		uint64_t nTime;
		uint64_t nByte;
	};

	class CB_TxPack final {
		RSA_t* TxUser;
		RSA_t* TxSign;

	public:
		void refreshTxInfo();
	};
}

#endif