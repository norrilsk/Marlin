#include "Marlin.hpp"
#include<iostream>
Marlin::Marlin(std::string path_to_data, std::string path_to_conf ): config(path_to_conf), log(config.get_log_ref()), mmu(config)
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
        Oper oper;
        mmu.read_from_mem(&instr,pc,1);
        //decode
        //oper = decode(instr);
        clocks++;
        break;
    }
}

Oper Marlin::decode(uint32_t instr)
{
    Oper* op;
    op = new(Oper);
    uint32_t opcode = instr & 0b1111111;
    switch (opcode)
    {
        case 0b0110111:
            (void)op;//  op= Oper//ты хотел перенести это в Oper как насчет дружественного класса decoder ?
            
            
            break;
        default:
            throw -1;
    
    }
    return *op;
}