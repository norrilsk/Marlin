#include "Memory.hpp"



Memory::Memory( Config& config ): config(config), log(config.get_log_ref())
{
    size = this->config.get_mem_size();
    memory.resize(size);
}

void Memory::write(void *data, uint64_t address, uint64_t num_bytes)
{
    if (address  +  num_bytes > size)
    {
        std::cerr<<"error: address is out of memory "<<std::endl;
        throw 30;
    }
    std::memcpy(&memory[address], data, num_bytes);
}

void Memory::read(void *data, uint64_t address, uint64_t num_bytes)
{
    if (address  +  num_bytes > size)
    {
        std::cerr<<"error: address is out of memory "<<std::endl;
        throw 30;
    }
    std::memcpy(data,&memory[address], num_bytes);
}