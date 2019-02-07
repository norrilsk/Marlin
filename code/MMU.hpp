
#ifndef MARLIN_MMU_HPP
#define MARLIN_MMU_HPP

#include "Memory.hpp"
#include "Config.hpp"
class MMU
{
private:
    Config config;
    Memory memory;
public:
    int upload_segment_to_mem(void* data,uint64_t addres, uint64_t flags,uint64_t data_size, uint64_t data_size_in_mem);
    MMU(Config config);
    MMU(){};
    ~MMU(){};
};

#endif //MARLIN_MMU_HPP
