#ifndef MARLIN_MARLIN_H
#define MARLIN_MARLIN_H

#include<string>

#include"ElfMarlin.hpp"
#include"MMU.hpp"
#include "Oper.hpp"
#include"Config.hpp"
#include"Decoder.hpp"
#include "Cell.hpp"
enum AccessType
{
    ACCESS_TYPE_READ,
    ACCESS_TYPE_WRITE
};
class Regfile
{
private:
    uint32_t num_regs = 0;
    std::vector<Register> regs;
    std::vector<uint32_t> dirtiness;
    Config& config;
public:
    Register get_reg(uint32_t num, AccessType acc = ACCESS_TYPE_READ);
    Register& get_reg_ref(uint32_t num, AccessType acc = ACCESS_TYPE_READ);
    uint32_t is_dirty(uint32_t num){return dirtiness[num];}
    void write_reg(Register reg ,uint32_t num){regs[num] = reg;}
    explicit Regfile(Config& config);
    ~Regfile() = default;
    
};

struct FD //Fetch-Decode
{
    bool is_stall = true;
    uint32_t pc = 0;
    uint32_t instr = 0;
};
struct DE //Decode -Execute
{
    bool is_stall = true;
    Oper* op;
};

class Marlin
{
private:
    Config config;
    Log::Loger& log;
    Decoder decoder;
    MMU mmu;
    Regfile regfile;
    uint64_t pc;
    Cell <FD> fd_cell; //fetch-decode cell
    Cell <DE> de_cell;
    uint64_t op_mode = 0;// 1 -x32 2-x64(unsupported)
    uint64_t ic = 0;// instruction counter
    uint64_t clocks = 0;
    void fetch();
    void decode();
public:
    void run();
    
    explicit Marlin( std::string  path_to_test, std::string path_to_conf );
    ~Marlin() = default;
    
};




#endif //MARLIN_MARLIN_H
