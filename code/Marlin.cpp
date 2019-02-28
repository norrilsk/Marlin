#include "Marlin.hpp"
#include<iostream>

Regfile::Regfile(Config& config) : config(config)
{
    num_regs = static_cast<uint32_t >(this->config.get_num_regs());
    dirtiness.resize(num_regs,0);
    regs.resize(num_regs);
    for (uint32_t i = 0; i < num_regs; i++)
    {
        regs[i].name = i;
    }
}
Register Regfile::get_reg(uint32_t num, AccessType acc)
{
    if (acc == ACCESS_TYPE_WRITE)
        dirtiness[num]--;
    return regs[num];
}
Register& Regfile::get_reg_ref(uint32_t num, AccessType acc)
{
    if (acc == ACCESS_TYPE_WRITE)
        dirtiness[num]--;
    return regs[num];
    
}

void Regfile::write_reg(Register reg )
{
    uint32_t num = reg.get_name();
    if ( num > this->num_regs)
        throw 1;
    regs[num] = reg;
    dirtiness[num]++;
}

Marlin::Marlin(std::string path_to_data, std::string path_to_conf ): config(path_to_conf), log(config.get_log_ref()),
                   mmu(config), regfile(config), hazartUnit(config,fd_cell,de_cell,em_cell,mw_cell), decoder(config,hazartUnit)
{
    
    ElfMarlin elf(path_to_data.c_str(),  config.get_log_marlin());
    pc = elf.get_entry32();
    op_mode = static_cast<uint64_t >(elf.get_i_class());
    if (op_mode != 1)
    {
        std::cerr<<"wrong operation mode(64 bit unsupported yet)"<<std::endl;
        throw 99;
    }
    elf.load(mmu);
    pc = elf.get_entry32();
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
        
        fd_cell.update();
        de_cell.update();
        em_cell.update();
        mw_cell.update();
        pc+=4;
        clocks++;
    }
}

void Marlin::fetch()
{
    FD* fd = fd_cell.get_store_ptr();
    uint32_t  instr;
    mmu.read_from_mem(&instr,pc,4);
    
    fd->pc = static_cast<uint32_t>(pc);
    fd->instr = instr;
    fd->is_stall = false;
}
void Marlin::decode()
{
    FD* fd = fd_cell.get_load_ptr();
    DE* de = de_cell.get_store_ptr();
    de->is_stall = fd->is_stall;
    if(fd->is_stall)
    {
        return;
    }
    Oper* op;
    op = decoder.decode32i(fd->instr, regfile);
    de->op = op;
    de->pc = fd->pc;
}
void Marlin::execute()
{
    DE* de = de_cell.get_load_ptr();
    EM* em = em_cell.get_store_ptr();
    em->is_stall = de->is_stall;
    if(de->is_stall)
    {
        return;
    }
    Oper* oper = de->op;
    oper->execute(de);
    
    em->pc = de->pc;
    em->op = de->op;
    em->is_stall = de->is_stall;
}
void Marlin::memory_access()
{
    EM* em = em_cell.get_load_ptr();
    MW* mw = mw_cell.get_store_ptr();
    mw->is_stall = em->is_stall;
    if (em->is_stall)
    {
        return;
    }
    mw->op = em->op;
    mw->pc = em->pc;
}

void Marlin::write_back()
{
    MW* mw = mw_cell.get_load_ptr();
    Oper* oper = mw->op;
    if (mw->is_stall)
    {
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