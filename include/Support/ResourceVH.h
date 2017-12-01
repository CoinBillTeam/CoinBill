#ifndef COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER
#define COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER

#include <Support/BasicUtility.h>
#include <string>
#include <fstream>
#include <bitset>

namespace CoinBill {
    // The basic Resource Value Handler.
    // We can abstractly access to data, such from file, etc.
    template <class Ty>
    class NOVTABLE ResourceVH {
    protected:
        Ty* pResource;

    public:
        virtual Ty* operator->() = 0;
    };

    // ResourceSecureVH class will help you to handle values more safely.
    // We will update / check crc value every time when you accessing it.
    // Note that will very slow when you accessing.
    // so make sure you are accessing this variable as multiple times.
    template <class Ty>
    class ResourceSecureVH : public ResourceVH<Ty> {
        // Not implemented yet.
    public:
    };

    template <class Ty>
    class ResourceMappedVH : public ResourceVH<Ty> {
        bool isCached;
        bool isMapped;

    public:
    };
}


#endif // end define COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER