#ifndef MARLIN_MARLIN_H
#define MARLIN_MARLIN_H

#include<string>
#include <chrono>
#include <ctime>

#include"ElfMarlin.hpp"
#include"MMU.hpp"
#include "Oper.hpp"
#include"Config.hpp"
#include"Decoder.hpp"
#include "Cell.hpp"
#include "HazartUnit.hpp"
#include "Regfile.hpp"

enum ExtendType
{
    EXTEND_TYPE_SIGN,
    EXTEND_TYPE_ZERO
};

class Marlin
{
private:
    Config config;
    Log::Loger& log;
    Log::Loger& trace;
    MMU mmu;
    Regfile regfile;
    Cell <WF>  fetch_cell;
    Cell <FD> fd_cell; //fetch-decode cell
    Cell <DE> de_cell; //decode-execute cell
    Cell <EM> em_cell; //execute-memory access cell
    Cell <MW> mw_cell; //memory access - write back cell
    HazartUnit hazartUnit;
    Decoder decoder;
    uint64_t op_mode = 0;// 1 -x32 2-x64(unsupported)
    uint64_t ic = 0;// instruction counter
    uint64_t clocks = 0;
    bool is_stop = false;
    bool is_dump_trace = false;
    long int  execution_time;
    int32_t  sign_extend(int32_t num, int32_t size, ExtendType extend_type);
    void dump_instruction(Oper* op);
    void dump_stat();
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
