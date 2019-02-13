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
    log << Log::err<<"test"<<" mess"<< "lol" << Log::endl<<"mess";
    elf.load(mmu);
}

