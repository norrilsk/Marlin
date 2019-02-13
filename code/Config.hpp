#ifndef MARLIN_CONFIG_HPP
#define MARLIN_CONFIG_HPP

#include <cstdint>
#include "Log.hpp"
class Config
{
private:
    uint64_t page_size = 0;
    uint64_t memory_size = page_size* 1024;
    std::string path_to_log;
    bool marlin_log = true;
    Log::Loger log;
   
public:
    uint64_t get_mem_size(){ return memory_size;};
    uint64_t get_page_size(){ return page_size;};
    bool get_log_marlin() {return marlin_log;};
    Log::Loger& get_log_ref() {return log;};
    void read_config_file(std::string path);
    Config() = default;
    explicit Config(std::string path);
    ~Config() = default ;
};

#endif //MARLIN_CONFIG_HPP
