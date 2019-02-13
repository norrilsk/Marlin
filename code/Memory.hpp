#ifndef MARLIN_MEMORY_HPP
#define MARLIN_MEMORY_HPP
#include <cstdint>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstring>

#include "Config.hpp"
#include "Log.hpp"
class Memory
{
private:
    Config& config;
    Log::Loger& log;
    uint64_t size =0;// total memory size in bytes
    std::vector<char> memory;
public:
    //Memory(){};
    explicit Memory(Config& config);
    ~Memory() = default;
    void upload(void* data, uint64_t addres, uint64_t num_bytes);
};

#endif //MARLIN_MEMORY_HPP
