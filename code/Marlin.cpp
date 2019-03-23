#include "Marlin.hpp"
#include<iostream>

Marlin::Marlin(std::string path_to_data, std::string path_to_conf ): config(path_to_conf), log(config.get_log_ref()),
                   mmu(config), regfile(config), hazartUnit(config, regfile, fd_cell,de_cell,em_cell,mw_cell,fetch_cell), decoder(config,hazartUnit)
{
    
    ElfMarlin elf(path_to_data.c_str(),  config.get_log_marlin());
   
    op_mode = static_cast<uint64_t >(elf.get_i_class());
    if (op_mode != 1)
    {
        std::cerr<<"wrong operation mode(64 bit unsupported yet)"<<std::endl;
        throw 99;
    }
    elf.load(mmu);
    
    //we do not have OS, so we have to initialize SP
    Register sp =regfile.get_reg(REGISTER_NAME_SP, ACCESS_TYPE_WRITE);
    sp.set_value(config.get_mem_size() - 5);
    
    // TODO: we have to initialize sp pointing to argc , argv
    uint32_t argc = 0;
    mmu.write_to_mem(&argc, sp.get_value(),4);
    regfile.write_reg(sp);
    
    
    WF* wf = fetch_cell.get_store_ptr();
    wf->pc = static_cast<uint32_t >(elf.get_entry32());
    wf->is_stall = false;
    fetch_cell.update();
}

void Marlin::run()
{
    while(1)
    {
        write_back();
        memory_access();
        execute();
        decode();
        fetch();
        
        fetch_cell.update();
        fd_cell.update();
        de_cell.update();
        em_cell.update();
        mw_cell.update();
        clocks++;
    }
}

void Marlin::fetch()
{
    WF* wf = fetch_cell.get_load_ptr();
    WF* wf_store = fetch_cell.get_store_ptr();
    FD* fd = fd_cell.get_store_ptr();
    uint32_t  instr;
    fd->is_stall =wf->is_stall || wf->is_hazard_stall;
    if (fd->is_stall || fd_cell.is_stop())
    {
        return;
    }
    uint32_t  pc = wf->is_jump? wf->pc_jump :wf->pc;
    mmu.read_from_mem(&instr,pc,4);
    
    fd->pc = static_cast<uint32_t>(pc);
    fd->instr = instr;
    fd->is_stall = false;
    wf_store->pc = pc +4;
}
void Marlin::decode()
{
    FD* fd = fd_cell.get_load_ptr();
    DE* de = de_cell.get_store_ptr();
    de->is_stall = fd->is_stall || fd->is_hazard_stall;
    if(de->is_stall || de_cell.is_stop())
    {
        return;
    }
    Oper* op;
    op = decoder.decode32i(fd->instr, regfile);
    op->set_pc(fd->pc);
    if (de_cell.is_stop())
    {
        //it means that registers are not ready now, we need to wait
        // and fix regfile
        hazartUnit.fix_dirtness(op);
    }
    de->op = op;
    de->pc = fd->pc;
}
void Marlin::execute()
{
    DE* de = de_cell.get_load_ptr();
    EM* em = em_cell.get_store_ptr();
    em->is_stall = de->is_stall || de->is_hazard_stall;
    Oper* oper = de->op;
    if(em->is_stall || em_cell.is_stop())
    {
        if (!de->is_stall  && de->is_hazard_stall)
            hazartUnit.fix_dirtness(oper);
        return;
    }
    oper->execute();
    if ( oper->is_oper_branch() &&
        (OPER_TYPE_J == oper->get_type() ||
        OPER_TYPE_I == oper->get_type()  ||
            (OPER_TYPE_B == oper->get_type() && dynamic_cast<OperB*>(oper)->oper_br_is_taken())))
    {
            hazartUnit.branch_hazart(oper);
    }
    
    em->pc = de->pc;
    em->op = de->op;
    em->is_stall = de->is_stall;
}
void Marlin::memory_access()
{
    EM* em = em_cell.get_load_ptr();
    MW* mw = mw_cell.get_store_ptr();
    Oper* oper = em->op;
    mw->is_stall = em->is_stall || em->is_hazard_stall;
    if (mw->is_stall || mw_cell.is_stop())
    {
        if (!em->is_stall  && em->is_hazard_stall)
            hazartUnit.fix_dirtness(oper);
        return;
    }
    AccessType op_acc_type = oper->get_mem_acc_type();
    OperType  op_type = oper->get_type();
    if ((ACCESS_TYPE_WRITE == op_acc_type )&&(OPER_TYPE_S == op_type))
    {
        OperS* op_s = dynamic_cast<OperS*>(oper);
        int32_t data = op_s->get_rs2().get_value();
        uint64_t addr = static_cast<uint64_t>(op_s->get_store_addr());
        uint64_t size = static_cast<uint64_t>(op_s->get_store_size());
        mmu.write_to_mem(&data,addr,size);
    }
    if ((ACCESS_TYPE_READ == op_acc_type) &&(OPER_TYPE_I == op_type))
    {
        OperI* op_i = dynamic_cast<OperI*>(oper);
        int32_t data = 0;
        uint64_t addr = static_cast<uint64_t>(op_i->get_load_addr());
        uint64_t size = static_cast<uint64_t>(op_i->get_load_size());
        mmu.read_from_mem(&data,addr,size);
        OperName name = op_i->get_name();
        if (OPER_NAME_LHU == name
            || OPER_NAME_LBU ==name)
        {
            data = sign_extend(data, size, EXTEND_TYPE_ZERO);
        }
        else if ( OPER_NAME_LH == name
                || OPER_NAME_LB == name
                || OPER_NAME_LW == name
                )
        {
            data = sign_extend(data, size, EXTEND_TYPE_SIGN);
        }
        else
            throw 1;
    
        oper->get_rd_ref().set_value(data);
    }
    
    mw->op = em->op;
    mw->pc = em->pc;
}

void Marlin::write_back()
{
    MW* mw = mw_cell.get_load_ptr();
    Oper* oper = mw->op;
    if (mw->is_stall || mw->is_hazard_stall)
    {
        if (!mw->is_stall  && mw->is_hazard_stall)
            hazartUnit.fix_dirtness(oper);
        return;
    }
    if (oper->get_type() == OPER_TYPE_R
        || oper->get_type() == OPER_TYPE_I
        || oper->get_type()== OPER_TYPE_U
        || oper->get_type() == OPER_TYPE_J
        )
    {
        regfile.write_reg(oper->get_rd());
    }
}


int32_t Marlin::sign_extend(int32_t n, int32_t size, ExtendType extend_type)
{
    if (4 == size)
        return n;
    uint32_t  num  = static_cast<uint32_t >(n);
    if (EXTEND_TYPE_ZERO == extend_type)
    {
        uint32_t mask = static_cast<uint32_t >((0b1 << size) - 1);
        return static_cast<int32_t >(mask & num);
    }
    else if ( EXTEND_TYPE_SIGN == extend_type)
    {
        uint32_t sign_bit = static_cast<uint32_t >(num >> (size - 1)) &0b1;
        uint32_t mask = static_cast<uint32_t >((0b1 << size) - 1);
        if ( sign_bit)
        {
            return static_cast<int32_t >(~mask | num);
        }
        else
        {
            return static_cast<int32_t >(mask & num);
        }
    }
    else
        throw 1;
}