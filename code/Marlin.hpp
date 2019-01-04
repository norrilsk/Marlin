#ifndef MARLIN_MARLIN_H
#define MARLIN_MARLIN_H

#include<string>
#include"ElfMarlin.hpp"
#include"Memory.hpp"
class Marlin
{
private:
    Memory memory;
    bool log = false;
    uint64_t pc;
    uint64_t op_mode = 0;// 1 -x32 2-x64(unsupported)
public:
    explicit Marlin( std::string , bool log = false );
    ~Marlin() = default;
    
};




#endif //MARLIN_MARLIN_H
