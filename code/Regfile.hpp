#ifndef MARLIN_REGFILE_HPP
#define MARLIN_REGFILE_HPP

#include "Oper.hpp"
#include <vector>
#include "Config.hpp"


class Regfile
{
private:
    uint32_t num_regs = 0;
    std::vector<Register> regs;
    std::vector<int32_t> dirtiness;
    Config& config;
public:
    Register get_reg(uint32_t num, AccessType acc = ACCESS_TYPE_READ);
    Register& get_reg_ref(uint32_t num, AccessType acc = ACCESS_TYPE_READ);
    int32_t is_dirty(uint32_t num){return dirtiness[num];}
    void virtual_write_reg(Register reg ); //write without real write :) fix dirtness
    void write_reg(Register reg );
    explicit Regfile(Config& config);
    ~Regfile() = default;
    
};


#endif //MARLIN_REGFILE_HPP
