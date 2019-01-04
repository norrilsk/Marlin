#include "Marlin.hpp"
#include<iostream>
Marlin::Marlin(std::string path_to_data, bool log ): log(log)
{
    ElfMarlin elf(path_to_data.c_str(),  static_cast<int>(log));
    pc = elf.get_entry32();
    op_mode = static_cast<uint64_t >(elf.get_i_class());
    if (op_mode != 1)
    {
        std::cerr<<"wrong operation mode(64 bit unsupported yet)"<<std::endl;
        throw 99;
    }
    elf.load(memory);
}

