#ifndef COINBILL_SUPPORT_RESOURCE_SECURE_VALUE_HANDLER
#define COINBILL_SUPPORT_RESOURCE_SECURE_VALUE_HANDLER

namespace CoinBill {
    // ResourceSecureVH class will help you to handle values more safely.
    // We will update / check crc value every time when you accessing it.
    class ResourceSecureVH {

    public:
        void nodifyModified();
    };
}


#endif // end define COINBILL_SUPPORT_RESOURCE_SECURE_VALUE_HANDLER
