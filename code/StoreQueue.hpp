#ifndef MARLIN_STOREQUEUE_H
#define MARLIN_STOREQUEUE_H

#include "Oper.hpp"
#include "MMU.hpp"
#include "Config.hpp"
#include <list>
class Store
{
public:
    uint64_t addr = 0;
    uint64_t size = 0;
    uint32_t data = 0;
    uint32_t id = 0;
    void clear()
    {
        addr = 0;
        size = 0;
        data = 0;
        id = 0;
    }
};
class StoreQueue
{
private:
    uint32_t id = 0;
    uint32_t skip;
    MMU& mmu;
    Store store;
    std::list<Store> queue;
    Config& config;
    bool tryConcatenate(Store store);
public:
    const uint32_t size;
    StoreQueue(uint32_t size , MMU& mmu, Config& config);
    ~StoreQueue(){};
    void push(Oper* op);
    void update();
    uint32_t searchData(Oper* op);
};



#endif // !MARLIN_STOREQUEUE_H