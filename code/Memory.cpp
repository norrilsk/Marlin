#include "Memory.hpp"



Memory::Memory( Config config ): config(config)
{
    size = config.GetMemSize();
    memory.resize(size);
}

void Memory::upload(void *data, uint64_t address, uint64_t num_bytes)
{
    if (address  +  num_bytes > size)
    {
        std::cerr<<"error: address is out of memory "<<std::endl;
        throw 30;
    }
    std::memcpy(&memory[address], data, num_bytes);
}
