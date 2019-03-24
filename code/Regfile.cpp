#include "Regfile.hpp"


Regfile::Regfile(Config& config) : config(config)
{
    num_regs = static_cast<uint32_t >(this->config.get_num_regs());
    dirtiness.resize(num_regs,0);
    regs.resize(num_regs);
    for (uint32_t i = 0; i < num_regs; i++)
    {
        regs[i].name = RegName(i);
    }
}
Register Regfile::get_reg(RegName num, AccessType acc)
{
    if (acc == ACCESS_TYPE_WRITE && REGISTER_NAME_ZERO!= num)
        dirtiness[num]--;
    return regs[num];
}
Register& Regfile::get_reg_ref(RegName num, AccessType acc)
{
    if (acc == ACCESS_TYPE_WRITE && REGISTER_NAME_ZERO!= num)
        dirtiness[num]--;
    return regs[num];
    
}

void Regfile::write_reg(Register reg )
{
    uint32_t num = reg.get_name();
    if ( num > this->num_regs)
        throw 1;
    if (REGISTER_NAME_ZERO == num)
        return;
    regs[num] = reg;
    dirtiness[num]++;
}

void Regfile::virtual_write_reg(Register reg)
{
    uint32_t num = reg.get_name();
    if ( num > this->num_regs)
        throw 1;
    if (REGISTER_NAME_ZERO == num)
        return;
    dirtiness[num]++;
}