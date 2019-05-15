#include "StoreQueue.hpp"


template<class T>
const T& min(const T& a, const T& b)
{
    return (b < a) ? b : a;
}

template<class T>
const T& max(const T& a, const T& b)
{
    return (a < b) ? b : a;
}

StoreQueue::StoreQueue(uint32_t size, MMU& mmu, Config& config):mmu(mmu),  config(config), size(size)
{
    skip = size;
}


void StoreQueue::push(Oper* op)
{
    if (op->get_mem_acc_type() != ACCESS_TYPE_WRITE)
        throw -1;
    OperS* op_s = dynamic_cast<OperS*>(op);
    store.data = static_cast<uint32_t>(op_s->get_rs2().get_value());
    store.addr = static_cast<uint64_t>(op_s->get_store_addr());
    store.size = static_cast<uint64_t>(op_s->get_store_size());
    id++;
    store.id = id;
    if (tryConcatenate(store))
    {
        store.clear();
    }
}

uint32_t StoreQueue::searchData(Oper* op)
{
    if (op->get_mem_acc_type() != ACCESS_TYPE_READ)
        throw -1;
    OperI* op_i = dynamic_cast<OperI*>(op);
    
    uint32_t id = queue.begin()->id;
    Store ld;
    ld.data = 0;
    ld.addr = static_cast<uint64_t>(op_i->get_load_addr());
    ld.size = static_cast<uint64_t>(op_i->get_load_size());
    std::vector<bool> fullness(32,false);
    
    for (auto it = queue.begin(); it != queue.end(); it++)
    {
        if (it->id < id)
            throw 1;
        id = it->id;
       
        
        if ((it->addr + it->size) >= ld.addr &&
            (it->addr + it->size) <= (ld.addr + ld.size))
        {
            if (ld.addr >= it->addr)
            {
                std::memcpy( (&ld.data), (char*)(&(it->data)) + ld.addr - it->addr,it->addr + it->size - ld.addr);
                
                for (uint32_t index = 0; index < it->addr + it->size - ld.addr; index++)
                {
                    fullness[index] = true;
                }
            }
            else
            {
                std::memcpy( (char*)(&ld.data) + (it->addr - ld.addr), &(it->data),it->size);
    
                for (uint64_t index =  (it->addr - ld.addr); index <  (it->addr - ld.addr) + it->size ; index++)
                {
                    fullness[index] = true;
                }
            }
            continue;
        
        }
    
        if ((it->addr) >= ld.addr &&
            (it->addr) <= (ld.addr + ld.size))
        {
            
            if (ld.addr + ld.size <= it->addr + it->size)
            {
                std::memcpy((char*)(&ld.data) + (it->addr - ld.addr), &(it->data),ld.addr + ld.size - it->addr );
                for (uint64_t index =  (it->addr - ld.addr); index <  (it->addr - ld.addr) + ld.addr + ld.size - it->addr ; index++)
                {
                    fullness[index] = true;
                }
            }
            else
            {
    
                std::memcpy( (char*)(&ld.data) + (it->addr - ld.addr), &(it->data),it->size);
    
                for (uint64_t index =  (it->addr - ld.addr); index <  (it->addr - ld.addr) + it->size ; index++)
                {
                    fullness[index] = true;
                }
            }
            continue;
        
        }
        if (ld.addr >= it->addr  &&
            ld.addr+ld.size  <= it->addr + it->size)
        {
            std::memcpy(&ld.data,(char*)(&it->data) + (ld.addr - it->addr), ld.size);
            for(unsigned idx = 0; idx < fullness.size(); idx++)
            {
                fullness[idx] = true;
            }
            continue;
        }
        
    }
    uint32_t mask  = 0;
    for ( uint32_t i = 0 ; i < fullness.size(); i++)
    {
        if (!fullness[i])
        {
            mask += 1 <<i;
            
        }
    }
    uint32_t d_data;
    mmu.read_from_mem(&d_data,ld.addr,ld.size);
    ld.data+= mask&d_data;
    return ld.data;
}

bool StoreQueue::tryConcatenate(Store st)
{
    bool res = false;
    uint32_t id = queue.rbegin()->id;
    for (auto it = queue.rbegin(); it != queue.rend(); it++)
    {
        if (it->id > id)
            throw 1;
        id = it->id;

        if ((it->addr + it->size) >= st.addr &&
            (it->addr + it->size) <= (st.addr + st.size))
        {
            uint64_t addr = min(it->addr, st.addr);
            uint64_t size = st.addr + st.size - addr;
            if (size > 32)
                return res;
            st.addr = addr;
            st.size = size;
            st.id = it->id;
            if (st.addr >= it->addr)
            {
                st.data = (st.data>>(st.addr - it->addr) );
                std::memcpy(&st.data, &(it->data), st.addr - it->addr);
                *it = st;
            }
            else
            {
                uint32_t data = st.data;
               //std::memcpy( (char*)(&data) +  (it->addr - st.addr), &it->data, it->size);
               st.data = data;
               *it = st;
            }
            res = true;
            continue;

        }

        if ((it->addr) >= st.addr &&
            (it->addr) <= (st.addr + st.size))
        {
            uint64_t addr = st.addr;
            uint64_t size = max(it->addr + it->size, st.addr +st.size) - addr;
            if (size > 32)
                return res;
            st.addr = addr;
            st.size = size;
            st.id = it->id;
            if (st.addr + st.size <= it->addr + it->size)
            {
                uint32_t data = it->data >> (it->addr - st.addr);
                std::memcpy(&data, &(st.data), st.size);
                st.data = data;
                *it = st;
            }
            else
            {
                uint32_t data = st.data;
                //std::memcpy((char*)(&data) + (it->addr - st.addr), &it->data, it->size);
                st.data = data;
                *it = st;
                //std::memcpy((char*)(&st.data) + (it->addr - st.addr), &st.data, st.size);
            }
            res = true;
            continue;

        }
        if (st.addr >= it->addr  &&
            st.addr+st.size  <= it->addr + it->size)
        {
            uint64_t addr = it->addr;
            uint64_t size = it->size;
            uint32_t  data = it->data;
            st.addr = addr;
            st.size = size;
            st.id = it->id;
            std::memcpy((char*)(&data) + ( st.addr - it->addr ), &(st.data) , st.size);
            st.data = data;
            *it = st;
            res = true;
            continue;
        }
         
        
    }
    return res;
}

void StoreQueue::update()
{
    queue.push_back(store);
    if (skip > 0)
    {
        skip--;
    }
    else
    {
        Store store = queue.front();
        if (store.size >0)
        {
            mmu.write_to_mem(&store.data, store.addr, store.size);
        }
        queue.pop_front();
    }
    store.clear();
}