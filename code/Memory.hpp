#ifndef MARLIN_MEMORY_HPP
#define MARLIN_MEMORY_HPP
#include <cstdint>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstring>
class Memory
{
private:
    
    uint64_t size =0;// total memory size in bytes
    std::vector<std::vector<char>> memory; //void* because memory shouldn't know about data type
    std::vector<uint64_t> block_sizes;
    std::vector<uint64_t> v_addresses;
    std::vector<uint64_t> p_addresses;
    std::vector<uint64_t> flags;
public:
    Memory(){};
    ~Memory() = default;
    void upload_v(void* data, uint64_t vadres, uint64_t flags, uint64_t num_bytes,
        uint64_t num_bytes_in_mem); // upload by virt address. use only for elf!!
    void upload_v(void* data, uint64_t vadres, uint64_t flags, uint64_t num_bytes);
    
};

#endif //MARLIN_MEMORY_HPP
