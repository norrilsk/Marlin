#ifndef MARLIN_HAZARTUNIT_HPP
#define MARLIN_HAZARTUNIT_HPP
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
    Cell <MW>& mw_cell; //memory access - write back cell
    Cell <WF>& wf_cell; //special cell for fetch take data from
    inline bool is_oper_load(Oper* op);
public:
    HazartUnit(Config& , Cell <FD>&,  Cell <DE>&, Cell <EM>&, Cell <MW>&, Cell <WF>& );
    Register hazart_in_decode(Register rs);
    void branch_hazart(OperB* oper, uint32_t pc);
    ~HazartUnit();
};
#endif
