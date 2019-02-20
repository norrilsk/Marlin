#include "Marlin.hpp"
#include<iostream>

Regfile::Regfile(uint32_t num) : num_regs(num)
{
    regs.resize(num);
}

Marlin::Marlin(std::string path_to_data, std::string path_to_conf ): config(path_to_conf), log(config.get_log_ref()), mmu(config),decoder(config)
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
        //Fetch
        uint32_t instr;
        mmu.read_from_mem(&instr,pc,1);
        //decode
        Oper* oper;
        oper = decoder.decode32i(instr);
        clocks++;
        break;
    }
}
