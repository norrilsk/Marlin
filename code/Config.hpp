#ifndef MARLIN_CONFIG_HPP
#define MARLIN_CONFIG_HPP

#include <cstdint>
class Config
{
private:
    uint64_t page_size = 4096;
    uint64_t memory_size = page_size* 1024;
    bool marlin_log;
public:
    uint64_t GetMemSize(){ return memory_size;};
    uint64_t GetPageSize(){ return page_size;};
    bool GetLogMarlin() {return marlin_log;};
    Config(){};
    ~Config(){};
};

#endif //MARLIN_CONFIG_HPP
