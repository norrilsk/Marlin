#include "Config.hpp"
#include "Cell.hpp"
#include "Oper.hpp"
class HazartUnit
{
    //statistics
    uint32_t bypasses_mem_ex = 0;
    uint32_t bypasses_wb_ex = 0;

    Config& config;
    Cell <FD>& fd_cell; //fetch-decode cell
    Cell <DE>& de_cell; //decode-execute cell
    Cell <EM>& em_cell; //execute-memory access cell
    Cell <MW>& mw_cell; //meory access - write back cell
    bool is_oper_load(Oper* op);
public:
    HazartUnit(Config& , Cell <FD>&,  Cell <DE>&, Cell <EM>&, Cell <MW>&);
    Register hazart_in_decode(Register rs);
    ~HazartUnit();
};

