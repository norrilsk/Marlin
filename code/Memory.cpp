#include "Memory.hpp"

void Memory::upload_v(void *data, uint64_t vaddress, uint64_t flags, uint64_t num_bytes)
{
    return upload_v(data, vaddress,flags, num_bytes, num_bytes);
}

void Memory::upload_v(void *data, uint64_t vaddress, uint64_t flags, uint64_t num_bytes, uint64_t num_bytes_in_mem)
{
    //check if this address is not empty
    for (uint64_t i = 0; i < v_addresses.size(); i++)
    {
        if ((v_addresses[i] < vaddress) && (v_addresses[i] + block_sizes[i] > vaddress))
        {
            //TODO:: replace memory
            throw 31;
            
        }
    }
    
    if (num_bytes_in_mem < num_bytes)
    {
        std::cerr<<"error: num_bytes_in_mem < num_bytes "<<std::endl;
        throw 30;
    }
    
    
    
    v_addresses.push_back(vaddress);
    memory.emplace_back(num_bytes_in_mem,0);
    std::memcpy(&memory[memory.size() -1][0], data, num_bytes_in_mem);
    block_sizes.push_back(num_bytes_in_mem);
    this->flags.push_back(flags);
    //p_addresses.push_back();
    
}
