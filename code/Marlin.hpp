#ifndef MARLIN_MARLIN_H
#define MARLIN_MARLIN_H

#include<string>

#include"ElfMarlin.hpp"
#include"MMU.hpp"
#include"Config.hpp"
class Marlin
{
private:
    Config config;
    MMU mmu;
    bool log = false;
    uint64_t pc;
    uint64_t op_mode = 0;// 1 -x32 2-x64(unsupported)
public:
    explicit Marlin( std::string , Config );
    ~Marlin() = default;
    
};




#endif //MARLIN_MARLIN_H
