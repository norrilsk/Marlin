#include "HazartUnit.hpp"


HazartUnit::HazartUnit(Config& config, Regfile& regfile, Cell <FD>& fd_cell, Cell <DE>& de_cell, Cell <EM>& em_cell, Cell <MW>& mw_cell,Cell <WF>& wf_cell):
    config(config), regfile(regfile), fd_cell(fd_cell), de_cell(de_cell), em_cell(em_cell), mw_cell(mw_cell), wf_cell(wf_cell)
{
    (void)this->config.get_page_size();
    (void)this->fd_cell.phase1;
    (void)this->de_cell.phase1;
}

Register HazartUnit::hazart_in_decode(Register rs)
{
    DE* de = de_cell.get_load_ptr();
    bool stall = de->is_stall || de->is_hazard_stall;
    Oper* oper = de->op;
    if (!stall &&
            (oper->get_type() == OPER_TYPE_R
            || oper->get_type() == OPER_TYPE_I
            || oper->get_type() == OPER_TYPE_U
            || oper->get_type() == OPER_TYPE_J)
        )
    {
        Register rd = oper->get_rd();
        if (rd.get_name() == rs.get_name())
        {
            if (is_oper_load(oper))
                throw 1;// unsupported yet 
            bypasses_mem_ex++;
            return rd;
        }
    }
    EM * em = em_cell.get_load_ptr();
    oper = em->op;
    stall = em->is_stall || em->is_hazard_stall;
    if (!stall &&
        (oper->get_type() == OPER_TYPE_R
            || oper->get_type() == OPER_TYPE_I
            || oper->get_type() == OPER_TYPE_U
            || oper->get_type() == OPER_TYPE_J)
        )
    {
        Register rd = oper->get_rd();
        if (rd.get_name() == rs.get_name())
        {
            bypasses_wb_ex++;
            return rd;
        }
    }
    throw 1;
    
}

void HazartUnit::branch_hazart(Oper *oper, uint32_t pc)
{
   WF * wf = wf_cell.get_store_ptr();
   wf->is_jump = true;
   wf->pc_jump = static_cast<int32_t >(oper->get_imm()) + static_cast<int32_t >(pc);
   FD * fd = fd_cell.get_store_ptr();
   fd->is_hazard_stall = true;
   DE* de = de_cell.get_store_ptr();
   de->is_hazard_stall = true;
   
}

inline bool HazartUnit::is_oper_load(Oper* op)
{
    return (op->get_mem_acc_type() == ACCESS_TYPE_READ);
}

void HazartUnit::fix_dirtness(Oper *op)
{
    OperType  type = op->get_type();
    if ( OPER_TYPE_R == type
        || OPER_TYPE_I == type
        || OPER_TYPE_U == type
         || OPER_TYPE_J == type)
    {
        regfile.virtual_write_reg(op->get_rd());
    }
}

HazartUnit::~HazartUnit()
{
}
