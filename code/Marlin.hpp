#ifndef MARLIN_MARLIN_H
#define MARLIN_MARLIN_H

#include<string>

#include"ElfMarlin.hpp"
#include"MMU.hpp"
#include "Oper.hpp"
#include"Config.hpp"
#include"Decoder.hpp"

class Regfile
{
private:
    uint32_t num_regs = 0;
    std::vector<Register> regs;
public:
    Register get_reg(uint32_t num){return regs[num];}
    Register& get_reg_ref(uint32_t num){ return regs[num]; }
    void write_reg(Register reg ,uint32_t num){regs[num] = reg;}
    Regfile(uint32_t num_regs);
    ~Regfile();
    
};


class Marlin
{
private:
    Config config;
    Log::Loger& log;
    Decoder decoder;
    MMU mmu;
    uint64_t pc;
    uint64_t op_mode = 0;// 1 -x32 2-x64(unsupported)
    uint64_t ic = 0;// instruction counter
    uint64_t clocks = 0;
public:
    void run();
    
    explicit Marlin( std::string  path_to_test, std::string path_to_conf );
    ~Marlin() = default;
    
};




#endif //MARLIN_MARLIN_H
