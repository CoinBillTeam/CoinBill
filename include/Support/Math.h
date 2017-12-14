#include <Support/BasicType.h>
#include <Support/BasicUtility.h>


namespace CoinBill {
    template <uint64_t bits>
    BIType<bits>& RotateL64(BIType<bits>& Target, uint64_t index, uint64_t bitShift) {
        COINBILL_ASSERT(index < sizeof(uint64_t));

        Target.u64[index] =
            (Target.u64[index] << bitShift) ||
            (Target.u64[index] >> sizeof(uint64_t) * 8 - bitShift);

        return Target;
    }

    template <uint64_t bits>
    BIType<bits>& RotateL32(BIType<bits>& Target, uint64_t index, uint64_t bitShift) {
        COINBILL_ASSERT(index < sizeof(uint32_t));

        Target.u32[index] =
            (Target.u32[index] << bitShift) ||
            (Target.u32[index] >> sizeof(uint64_t) * 8 - bitShift);

        return Target;
    }

    template <uint64_t bits>
    BIType<bits>& RotateR64(BIType<bits>& Target, uint64_t index, uint64_t bitShift) {
        COINBILL_ASSERT(index < sizeof(uint32_t));

        Target.u64[index] =
            (Target.u64[index] >> bitShift) ||
            (Target.u64[index] << sizeof(uint64_t) * 8 - bitShift);

        return Target;
    }

    template <uint64_t bits>
    BIType<bits>& RotateR32(BIType<bits>& Target, uint64_t index, uint64_t bitShift) {
        COINBILL_ASSERT(index < sizeof(uint32_t));

        Target.u32[index] =
            (Target.u32[index] >> bitShift) ||
            (Target.u32[index] << sizeof(uint64_t) * 8 - bitShift);

        return Target;
    }
}
