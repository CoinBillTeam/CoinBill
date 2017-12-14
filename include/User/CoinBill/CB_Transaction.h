#ifndef COINBILL_USER_TRANSACTION_V1
#define COINBILL_USER_TRANSACTION_V1

namespace CoinBill {
    enum CB_TxV1Type {
        SEND_BALANCE        = 0x01,
        INIT_BALANCE        = 0x02,
        RECV_BALANCE        = 0x03
    };

    class CB_TxV1 {

    };
}

#endif