#include "Oper.hpp"



Oper::Oper(OperName name, OperType type) :type(type), name(name)
{
}

OperI::OperI(OperName name) : Oper(name, OPER_TYPE_I)
{

}

OperR::OperR(OperName name) : Oper(name, OPER_TYPE_R)
{

}

OperS::OperS(OperName name) : Oper(name, OPER_TYPE_S)
{

}

OperB::OperB(OperName name) : Oper(name, OPER_TYPE_B)
{

}
OperU::OperU(OperName name) : Oper(name, OPER_TYPE_U)
{

}
OperJ::OperJ(OperName name) : Oper(name, OPER_TYPE_J)
{

}

void Oper::execute(DE* de)
{
    if (this->main_executor)
    {
        main_executor(this, de);
    }
    else
    {
        std::cerr << "no executor for operation " << this->name;
        throw -1;
    }
}

void Oper::calc_imm(uint32_t instr)
{
    (void)instr;
}
void OperI::calc_imm(uint32_t instr)
{
    uint32_t s1 = 0;
    if (((instr >> 31) & 0b1) == 0b1)
    {
        s1 = 0b111111111111111111111<<11;
    }
    uint32_t s2 = 0;
    s2 = ((instr >> 25) & 0b111111) <<5;
    uint32_t s3 = 0;
    s3 = ((instr >>21) & 0b1111) <<1;
    uint32_t s4 = 0;
    s4 = (instr >>20) & 0b1;
    imm = s1 +s2 +s3 +s4;

}

void OperS::calc_imm(uint32_t instr)
{
    uint32_t s1 = 0;
    if (((instr >> 31) & 0b1) == 0b1)
    {
        s1 = 0b111111111111111111111 << 11;
    }
    uint32_t s2 = 0;
    s2 = ((instr >> 25) & 0b111111) << 5;
    uint32_t s3 = 0;
    s3 = ((instr >> 8) & 0b1111) << 1;
    uint32_t s4 = 0;
    s4 = (instr >> 7) & 0b1;
    imm = s1 + s2 + s3 + s4;

}

void OperB::calc_imm(uint32_t instr)
{
    uint32_t s1 = 0;
    if (((instr >> 31) & 0b1) == 0b1)
    {
        s1 = 0b11111111111111111111 << 12;
    }
    uint32_t s2 = 0;
    s2 = ((instr >> 25) & 0b111111) << 5;
    uint32_t s3 = 0;
    s3 = ((instr >> 8) & 0b1111) << 1;
    uint32_t s4 = 0;
    s4 = ((instr >> 7) & 0b1) <<11;
    imm = s1 + s2 + s3 + s4;

}
void OperU::calc_imm(uint32_t instr)
{
    
    imm = instr & 0b11111111111111111111000000000000;
}

void OperJ::calc_imm(uint32_t instr)
{
    uint32_t s1 = 0;
    if (((instr >> 31) & 0b1) == 0b1)
    {
        s1 = 0b111111111111 << 20;
    }
    uint32_t s2 = 0;
    s2 = instr & (0b11111111 <<12);
    uint32_t s3 = ((instr >> 20) & 0b1) << 11;
    uint32_t s4 = ((instr >> 25) & 0b111111) << 5;
    uint32_t s5 = 0;
    s5 = ((instr >> 21) & 0b1111) << 1;
    imm = s1 +s2 +s3 +s4 +s5;
}

