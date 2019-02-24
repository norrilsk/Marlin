#include "Marlin.hpp"
#include<iostream>

Regfile::Regfile(Config& config) : config(config)
{
    auto num = static_cast<uint32_t >(config.get_num_regs());
    dirtiness.resize(num ,0);
    regs.resize(num);
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

Marlin::Marlin(std::string path_to_data, std::string path_to_conf ): config(path_to_conf), log(config.get_log_ref()),
                    decoder(config),mmu(config), regfile(config)
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
        fetch();
        decode();
        
        fd_cell.update();
        de_cell.update();
        pc+=4;
        clocks++;
    }
}

void Marlin::fetch()
{
    FD fd;
    fd.pc = static_cast<uint32_t>(pc);
    uint32_t  instr;
    mmu.read_from_mem(&instr,pc,4);
    fd.instr = instr;
    fd.is_stall = false;
    fd_cell.store(&fd);
}
void Marlin::decode()
{
    FD fd;
    DE de;
    fd_cell.load(&fd);
    de.is_stall = fd.is_stall;
    if(fd.is_stall)
    {
        de_cell.store(&de);
        return;
    }
    Oper* op;
    op = decoder.decode32i(fd.instr, regfile);
    de.op = op;
    de_cell.store(&de);
}