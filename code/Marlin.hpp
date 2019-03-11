#ifndef MARLIN_MARLIN_H
#define MARLIN_MARLIN_H

#include<string>

#include"ElfMarlin.hpp"
#include"MMU.hpp"
#include "Oper.hpp"
#include"Config.hpp"
#include"Decoder.hpp"
#include "Cell.hpp"
#include "HazartUnit.hpp"

class Regfile
{
private:
    uint32_t num_regs = 0;
    std::vector<Register> regs;
    std::vector<int32_t> dirtiness;
    Config& config;
public:
    Register get_reg(uint32_t num, AccessType acc = ACCESS_TYPE_READ);
    Register& get_reg_ref(uint32_t num, AccessType acc = ACCESS_TYPE_READ);
    int32_t is_dirty(uint32_t num){return dirtiness[num];}
    void write_reg(Register reg );
    explicit Regfile(Config& config);
    ~Regfile() = default;
    
};


class Marlin
{
private:
    Config config;
    Log::Loger& log;
    MMU mmu;
    Regfile regfile;
    Cell <WF>  fetch_cell;
    Cell <FD> fd_cell; //fetch-decode cell
    Cell <DE> de_cell; //decode-execute cell
    Cell <EM> em_cell; //execute-memory access cell
    Cell <MW> mw_cell; //meory access - write back cell
    HazartUnit hazartUnit;
    Decoder decoder;

    uint64_t op_mode = 0;// 1 -x32 2-x64(unsupported)
//    uint64_t ic = 0;// instruction counter
    uint64_t clocks = 0;
    void fetch();
    void decode();
    void execute();
    void memory_access();
    void write_back();
public:
    void run();
    
    explicit Marlin( std::string  path_to_test, std::string path_to_conf );
    ~Marlin() = default;
    
};




#endif //MARLIN_MARLIN_H
