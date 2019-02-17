#ifndef MARLIN_OPER_HPP
#define MARLIN_OPER_HPP
#include <cstdint>
enum OperType
{
    OPER_TYPE_R,
    OPER_TYPE_I,
    OPER_TYPE_S,
    OPER_TYPE_B,
    OPER_TYPE_U,
    OPER_TYPE_J,
    OPER_TYPE_NONE
};
enum OperName
{
    OPER_NAME_LUI,
    OPER_NAME_AUIPC,
    OPER_NAME_JAL,
    OPER_NAME_JALR,
    OPER_NAME_BEQ,
    OPER_NAME_BNE,
    OPER_NAME_BLT,
    OPER_NAME_BGE,
    OPER_NAME_BLTU,
    OPER_NAME_BGEU,
    OPER_NAME_LB,
    OPER_NAME_LH,
    OPER_NAME_LW,
    OPER_NAME_LBU,
    OPER_NAME_LHU,
    OPER_NAME_SB,
    OPER_NAME_SH,
    OPER_NAME_SW,
    OPER_NAME_ADDI,
    OPER_NAME_SLTI,
    OPER_NAME_SLTIU,
    OPER_NAME_XORI,
    OPER_NAME_ORI,
    OPER_NAME_ANDI,
    OPER_NAME_SLLI,
    OPER_NAME_SRLI,
    OPER_NAME_SRAI,
    OPER_NAME_ADD,
    OPER_NAME_SUB,
    OPER_NAME_SLL,
    OPER_NAME_SLT,
    OPER_NAME_SLTU,
    OPER_NAME_XOR,
    OPER_NAME_SRL,
    OPER_NAME_SRA,
    OPER_NAME_OR,
    OPER_NAME_AND,
    OPER_NAME_FENCE,
    OPER_NAME_FENCE_I,
    OPER_NAME_ECALL,
    OPER_NAME_EBREAK,
    OPER_NAME_CSRRW,
    OPER_NAME_CSRRS,
    OPER_NAME_CSRRC,
    OPER_NAME_CSRRWI,
    OPER_NAME_CSRRSI,
    OPER_NAME_CSRRCI,
    OPER_NAME_NONE,

};


class Register
{
private:
    uint32_t value;
public:
    uint32_t get_value(){return value;}
    Register(){};
    ~Register(){};



};



class Oper
{
    OperType type = OPER_TYPE_NONE;
    OperName name = OPER_NAME_NONE;

public:
    uint32_t opcode;
    
    Oper(){};
    Oper(OperName name, OperType type);
    
    OperName get_name(){ return name;}
    OperType get_type(){ return type;}
    ~Oper(){};
};

class OperR : public Oper
{
private:

public:
    uint32_t funct3;
    uint32_t funct7;
    Register rs1;
    Register rs2;
    Register rd;
    explicit OperR(OperName name);
    OperR(){};
    ~OperR() {};

};

class OperI: public Oper
{
public:
    uint32_t funct3;
    uint32_t imm;
    Register rs1;
    Register rd;
    explicit OperI(OperName name);
    OperI(){};
    ~OperI(){};
};

class OperS: public Oper
{
public:
    uint32_t funct3;
    uint32_t imm;
    Register rs1;
    Register rs2;
    explicit OperS(OperName name);
    OperS(){};
    ~OperS(){};
};
class OperB: public Oper
{
public:
    uint32_t funct3;
    uint32_t imm;
    Register rs1;
    Register rs2;
    explicit OperB(OperName name);
    OperB(){};
    ~OperB(){};
};

class OperU: public Oper
{
public:
    uint32_t imm;
    Register rd;
    explicit OperU(OperName name);
    OperU(){};
    ~OperU(){};
};

class OperJ: public Oper
{
public:
    uint32_t imm;
    Register rd;
    explicit OperJ(OperName name);
    OperJ(){};
    ~OperJ(){};
};





#endif