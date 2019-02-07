#ifndef MARLIN_READ_ELF_H
#define MARLIN_READ_ELF_H

#include<err.h>
#include<fcntl.h>
#include<gelf.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include <vector>

#include"MMU.hpp"


class ElfMarlin
{
    /*
     * 0 -ELFCLASSNONE
     * 1 -ELFCLASS32
     * 2 -ELFCLASSS64
     */
    int i_class  = 0;
    /*
     * 0 - ELFDATANONE
     * 1 - Little Endian
     * 2 - Big Endian
     */
    int i_data = 0;
    /*
     * 0  -undefined
     * 1 - relocate
     * 2 - execute
     * 3 - dynamic
     * 4 - core
     */
    int type =  0;
    int machine =  0;
    int flags = 0;
    uint64_t entry64 = 0;
    uint32_t entry32 = 0;
    Elf * e;
    int fd;
public:
    ElfMarlin(const char * str, int log);
    ~ElfMarlin();
    int get_i_class() {return i_class;}
    int get_i_data() {return i_data;}
    int get_type() {return type;}
    int get_machine() { return machine;}
    int get_flags() {return flags;}
    uint32_t get_entry32() { return entry32;}
    uint64_t get_entry64() { return entry64;}
    
    void load (MMU &mmu);
    bool check();
    
};
#endif //MARLIN_READ_ELF_H
