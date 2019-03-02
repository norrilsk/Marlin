#ifndef MARLIN_OPER_HPP
#define MARLIN_OPER_HPP
#include <cstdint>
#include <iostream>
#include "Cell.hpp"



enum AccessType
{
    ACCESS_TYPE_READ,
    ACCESS_TYPE_WRITE,
    ACCESS_TYPE_NONE
};
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
    int32_t value = 0;
    uint32_t name;
    friend  class Regfile;
public:
    int32_t get_value() { return value; }
    void set_value(int32_t val){value = val;}
    uint32_t get_name() {return name;}
    Register() = default;
    ~Register() = default;


};

class Oper
{
    OperType type = OPER_TYPE_NONE;
    OperName name = OPER_NAME_NONE;
    uint32_t opcode;
    AccessType mem_acc_type = ACCESS_TYPE_NONE;
    void(*main_executor)(Oper*, DE*) = nullptr;
    friend class Decoder;
public:
   
    AccessType get_mem_acc_type(){return mem_acc_type;}
    Oper() = default;
    Oper(OperName name, OperType type);
    virtual void calc_imm(uint32_t instr);
    virtual uint32_t get_imm(){throw 1;}
    virtual uint32_t get_f3() { throw 1; }
    virtual uint32_t get_f7() { throw 1; }
    virtual Register get_rs1() { throw 1; };
    virtual Register get_rs2() { throw 1; };
    virtual Register get_rd() {throw 1; };
    virtual Register& get_rd_ref() { throw 1; };
    void execute(DE*);
    OperName get_name(){ return name;}
    OperType get_type(){ return type;}
    virtual ~Oper() = default ;
};

class OperR : public Oper
{
private:
    uint32_t funct3;
    uint32_t funct7;
    Register rs1;
    Register rs2;
    Register rd;
    friend class Decoder;
public:
    uint32_t get_f3(){return funct3;}
    uint32_t get_f7(){return funct7;}
    Register get_rs1(){return rs1;};
    Register get_rs2(){return rs2;};
    Register get_rd() { return rd; };
    Register& get_rd_ref() { return rd; };
    void set_rd(Register rd) { this->rd = rd; };
    explicit OperR(OperName name);
    OperR()= default;
    ~OperR()  = default;

};

class OperI: public Oper
{
private:
    uint32_t funct3;
    uint32_t imm;
    Register rs1;
    Register rd;
    int32_t load_addres;
    int32_t load_size;
    friend class Decoder;
public:
    void calc_imm(uint32_t instr);
    uint32_t get_imm(){return imm;}
    uint32_t get_f3() { return funct3; }
    Register get_rs1() { return rs1; };
    Register get_rd() { return rd; };
    Register& get_rd_ref() { return rd; };
    void set_rd(Register rd) { this->rd = rd; };
    int32_t get_load_addr(){ return load_addres;}
    int32_t get_load_size(){ return load_size;}
    explicit OperI(OperName name);
    OperI()= default;
    ~OperI()= default;
};

class OperS: public Oper
{
private:
    uint32_t funct3;
    uint32_t imm;
    Register rs1;
    Register rs2;
    int32_t store_addres;
    int32_t store_size;
    friend class Decoder;
public:
    void calc_imm(uint32_t instr);
    uint32_t get_imm() { return imm; }
    uint32_t get_f3() { return funct3; }
    Register get_rs1() { return rs1; }
    Register get_rs2() { return rs2; }
    int32_t get_store_addr(){ return store_addres;}
    int32_t get_store_size(){ return store_size;}
    void set_store_addr(int32_t sa){store_addres=sa;}
    explicit OperS(OperName name);
    OperS()= default;
    ~OperS()= default;
};
class OperB: public Oper
{
private:
    uint32_t funct3;
    uint32_t imm;
    Register rs1;
    Register rs2;
    friend class Decoder;
public:
    void calc_imm(uint32_t instr);
    uint32_t get_imm() { return imm; }
    uint32_t get_f3() { return funct3; }
    Register get_rs1() { return rs1; }
    Register get_rs2() { return rs2; }
    explicit OperB(OperName name);
    OperB()= default;
    ~OperB()= default;
};

class OperU: public Oper
{
private:
    uint32_t imm;
    Register rd;
    friend class Decoder;
public:
    void calc_imm(uint32_t instr);
    Register get_rd() { return rd; };
    Register& get_rd_ref() { return rd; };
    void set_rd(Register rd){this->rd = rd;};
    uint32_t get_imm() { return imm; }
    explicit OperU(OperName name);
    OperU()= default;
    ~OperU()= default;
};

class OperJ: public Oper
{
private:
    uint32_t imm;
    Register rd;
    friend class Decoder;
public:
    void calc_imm(uint32_t instr);
    uint32_t get_imm() { return imm; }
    Register get_rd() { return rd; };
    Register& get_rd_ref() { return rd; };
    void set_rd(Register rd) { this->rd = rd; };
    explicit OperJ(OperName name);
    OperJ()= default;
    ~OperJ()= default;
};



#endif