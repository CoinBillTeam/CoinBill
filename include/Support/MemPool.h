#ifndef COINBILL_SUPPORT_MEMPOOL
#define COINBILL_SUPPORT_MEMPOOL

#include <mutex>
#include <list>
#include <Support/Basic.h>
#include <Support/Types.h>

namespace CoinBill {

    template <class Ty, unsigned int szRsvMem = 32, unsigned int szRsvList = 128>
    class MemPool {
        // Node Reserved Infomations.
        std::mutex          NodeMtx;
        Ty**                NodeLst; // Actual Node List. (First of Node)
        Ty**                NodeCur; // Current Node Index.
        unsigned int        ListSize; // Node List Size. (Maximum)

        // The pools that stay around on nodes.
        std::list<Ty*>      Pools;

    public:
        MemPool() : NodeLst(nullptr), NodeCur(nullptr), ListSize(0) {

        }
        ~MemPool() {
            delete NodeLst;
            for (Ty* pool : Pools) {
                delete pool;
            }
        }

        Ty* create() {
            // mutex lock guard for thread safe.
            std::lock_guard<std::mutex> mutex_gard(NodeMtx);

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
                Ty* newPool = new Ty[szRsvMem];
                Pools.push_back(newPool);

                // Initialize pool mems on list.
                unsigned int i = 0;
                for (; i < szRsvMem; ++i) {
                    NodeLst[i] = &newPool[i];
                }
                // set current node.
                NodeCur = &NodeLst[i];
            }
            Ty* newMem = *NodeCur;
            NodeCur = offset(NodeCur, -sizeof(Ty*));

            return newMem;
        }

        void distroy(Ty* object) {
            // mutex lock guard for thread safe.
            std::lock_guard<std::mutex> mutex_gard(NodeMtx);

            NodeCur = offset(NodeCur, sizeof(Ty*));
            *NodeCur = object;
        }
    }; 
}

template <class Ty>
void* operator new(size_t size, CoinBill::MemPool<Ty>& pool) {
    return pool.create();
}
template <class Ty>
void* operator new(size_t size, CoinBill::MemPool<Ty>* pool) {
    return pool->create();
}
template <class Ty>
void operator delete(void* ptr, CoinBill::MemPool<Ty>& pool) {
    pool.distroy((Ty*)ptr);
}
template <class Ty>
void operator delete(void* ptr, CoinBill::MemPool<Ty>* pool) {
    pool->distroy((Ty*)ptr);
}

#endif // COINBILL_SUPPORT_MEMPOOL