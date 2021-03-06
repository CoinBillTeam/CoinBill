#ifndef COINBILL_SUPPORT_RESOURCE_POOL
#define COINBILL_SUPPORT_RESOURCE_POOL

#include <mutex>
#include <list>
#include <Support/BasicUtility.h>

namespace CoinBill {

    template <class Ty, unsigned int szRsvMem = 32, unsigned int szRsvList = 128>
    class ResourcePool {
        // Node Reserved Infomations.
        Ty**                NodeLst; // Actual Node List. (First of Node)
        Ty**                NodeCur; // Current Node Index.
        unsigned int        ListSize; // Node List Size. (Maximum)

        // The pools that stay around on nodes.
        std::list<Ty*>      Pools;

    public:
        ResourcePool() : NodeLst(nullptr), NodeCur(nullptr), ListSize(0) { }
        ~ResourcePool() {
            delete NodeLst;
            for (Ty* pool : Pools) {
                delete pool;
            }
        }

        Ty* create() {
            // Check that memory list is empty.
            if (NodeCur == NodeLst) {
                // current node is first of node.
                // the node is emtpy. need reallocation.
                if(szRsvMem * Pools.size() == ListSize) {
                    // If list is full, reallocate list first.
                    // delete old node list.
                    delete NodeLst;
                    
                    // Now initialize with new node.
                    ListSize += szRsvList;
                    NodeLst = new Ty*[ListSize];
                    NodeCur = NodeLst;
                    // Node list refreshed.
                }

                // Allocate new pool.
                Ty* newPool = (Ty*)::operator new(round_up<4096>(sizeof(Ty) * szRsvMem));
                Pools.push_back(newPool);

                // Initialize pool mems on list.
                unsigned int i = 0;
                for (; i < szRsvMem; ++i) {
                    NodeLst[i] = &newPool[i];
                }
                // set current node.
                NodeCur = &(NodeLst[i - 1]);
            }
            Ty* newMem = *NodeCur;
            NodeCur = (Ty**)offset(NodeCur, -sizeof(Ty*));

            return newMem;
        }

        void distroy(Ty* object) {
            object->~Ty();
            NodeCur = (Ty**)offset(NodeCur, sizeof(Ty*));
            *NodeCur = object;
        }
    }; 

    template <class Ty, size_t FPadSize, size_t BPadSize>
    struct SecurePaddedType {
        uint8_t BPad[BPadSize];
        Ty      RV;
        uint8_t FPad[FPadSize];
    };

    template <class Ty, unsigned int szRsvMem = 32, unsigned int szRsvList = 128>
    class SecureResourcePool : ResourcePool<SecurePaddedType<typename Ty, 16, 16>, szRsvMem, szRsvList> {
        typedef SecurePaddedType<typename Ty, 16, 16>                                       STy;
        typedef ResourcePool<SecurePaddedType<typename Ty, 16, 16>, szRsvMem, szRsvList>    PTy;

    public:
        Ty* create() override {
            return &(PTy::create()->RV);
        }
        void distroy(Ty* object) override {
            uint8_t* chkBPad = offset(object, -16);
            uint8_t* chkFPad = offset(object, +16);

            for (unsigned int i = 0; i < 16; ++i)
                COINBILL_ASSERT(chkBPad[i] == 0 && chkFPad[i] == 0);

            PTy::distory((STy*)chkBPad);
        }
    };
}
#endif // COINBILL_SUPPORT_MEMPOOL