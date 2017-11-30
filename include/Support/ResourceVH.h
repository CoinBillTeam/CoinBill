#ifndef COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER
#define COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER

namespace CoinBill {
    class ResourceVH {

    };

    // ResourceSecureVH class will help you to handle values more safely.
    // We will update / check crc value every time when you accessing it.
    class ResourceSecureVH : public ResourceVH {

    public:
        // update checksum value, the accessing 
        void nodifyModified();

    };

    class ResourceCachedVH : public ResourceVH {

    };
}


#endif // end define COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER