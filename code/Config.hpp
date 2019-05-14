#ifndef MARLIN_CONFIG_HPP
#define MARLIN_CONFIG_HPP

#include <cstdint>
#include "Log.hpp"
class Config
{
private:
    uint64_t page_size = 0;
    uint64_t memory_size = page_size* 1024;
    uint32_t pipeline_stages = 0; 
    std::string path_to_log;
    std::string path_to_trace;
    uint64_t  num_regs =0;
    bool marlin_log = true;
    Log::Loger log;
    Log::Loger trace;
    bool dump_trace = false;
    bool enable_sq = false;
    uint32_t sq_size = 0;
   
public:
    uint64_t get_mem_size() const{ return memory_size;};
    uint64_t get_page_size()const{ return page_size;};
    uint64_t get_num_regs()const{return num_regs;};
    uint32_t get_num_of_pipeline_stages() const{return pipeline_stages;}
    uint32_t get_sq_size() const{return sq_size;}
    bool get_log_marlin() const{return marlin_log;};
    bool is_dump_trace() const { return  dump_trace;}
    bool is_sq_enabled() const {return enable_sq;}
    Log::Loger& get_log_ref() {return log;};
    Log::Loger&  get_trace_ref() {return trace;}
    void read_config_file(std::string path);
    Config() = default;
    explicit Config(std::string path);
    ~Config() = default ;
};

#endif //MARLIN_CONFIG_HPP
