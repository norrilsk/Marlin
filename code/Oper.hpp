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
    OPER_TYPE_SYSTEM,
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
enum RegName
{
    REGISTER_NAME_ZERO = 0,
    REGISTER_NAME_RA,
    REGISTER_NAME_SP,
    REGISTER_NAME_GP,
    REGISTER_NAME_TP,
    REGISTER_NAME_T0,
    REGISTER_NAME_T1,
    REGISTER_NAME_T2,
    REGISTER_NAME_S0,
    REGISTER_NAME_S1,
    REGISTER_NAME_A0,
    REGISTER_NAME_A1,
    REGISTER_NAME_A2,
    REGISTER_NAME_A3,
    REGISTER_NAME_A4,
    REGISTER_NAME_A5,
    REGISTER_NAME_A6,
    REGISTER_NAME_A7,
    REGISTER_NAME_S2,
    REGISTER_NAME_S3,
    REGISTER_NAME_S4,
    REGISTER_NAME_S5,
    REGISTER_NAME_S6,
    REGISTER_NAME_S7,
    REGISTER_NAME_S8,
    REGISTER_NAME_S9,
    REGISTER_NAME_S10,
    REGISTER_NAME_S11,
    REGISTER_NAME_T3,
    REGISTER_NAME_T4,
    REGISTER_NAME_T5,
    REGISTER_NAME_T6,
    REGISTER_NAME_NONE
    
};



namespace Executors
{
  void MainInstrExecutorLUI(Oper *op);
  void MainInstrExecutorAUIPC(Oper *op);
  void MainInstrExecutorJAL(Oper *op);
  void MainInstrExecutorJALR(Oper *op);
  void MainInstrExecutorBEQ(Oper *op);
  void MainInstrExecutorBNE(Oper *op);
  void MainInstrExecutorBLT(Oper *op);
  void MainInstrExecutorBGE(Oper *op);
  void MainInstrExecutorBLTU(Oper *op);
  void MainInstrExecutorBGEU(Oper *op);
  void MainInstrExecutorStore(Oper *op ); //SW SH SB
  void MainInstrExecutorLoad(Oper *op ); //LW LH LHU LB LBU
  void MainInstrExecutorADDI(Oper *op );
  void MainInstrExecutorSLTI(Oper *op );
  void MainInstrExecutorSLTIU(Oper *op );
  void MainInstrExecutorXORI(Oper *op );
  void MainInstrExecutorORI(Oper *op );
  void MainInstrExecutorANDI(Oper *op );
  void MainInstrExecutorSLLI(Oper *op );
  void MainInstrExecutorSRLI(Oper *op );
  void MainInstrExecutorSRAI(Oper *op );
  void MainInstrExecutorADD(Oper *op );
  void MainInstrExecutorSUB(Oper *op );
  void MainInstrExecutorSLL(Oper *op );
  void MainInstrExecutorSLT(Oper *op );
  void MainInstrExecutorSLTU(Oper *op );
  void MainInstrExecutorXOR(Oper *op );
  void MainInstrExecutorSRL(Oper *op );
  void MainInstrExecutorSRA(Oper *op );
  void MainInstrExecutorOR(Oper *op );
  void MainInstrExecutorAND(Oper *op );
  void MainInstrExecutorECALL(Oper * op);
}

class Register
{
private:
    int32_t value = 0;
    RegName name;
    friend  class Regfile;
public:
    int32_t get_value() { return value; }
    void set_value(int32_t val){value = val;}
    RegName get_name() {return name;}
    Register() = default;
    ~Register() = default;


};

class Oper
{
    OperType type = OPER_TYPE_NONE;
    OperName name = OPER_NAME_NONE;
    uint32_t opcode;
    uint32_t pc;
    AccessType mem_acc_type = ACCESS_TYPE_NONE;
    bool is_branch = false;
    uint32_t branch_addr = 0;
    void(*main_executor)(Oper*) = nullptr;
    friend class Decoder;
public:
    uint32_t get_pc() const{ return pc;}
    void set_pc(uint32_t pc) {this->pc =pc;}
    AccessType get_mem_acc_type(){return mem_acc_type;}
    bool is_oper_branch(){return is_branch;}
    uint32_t get_br_target_addr() const{  if (is_branch) return branch_addr; else  throw  1;};
    void set_br_target_addr(uint32_t addr) {branch_addr = addr; if (!is_branch ) throw 1;};
    
    Oper() = default;
    Oper(OperName name, OperType type);
    virtual void calc_imm(uint32_t instr);
    virtual uint32_t get_imm() const{throw 1;}
    virtual uint32_t get_f3() const{ throw 1; }
    virtual uint32_t get_f7() const{ throw 1; }
    virtual Register get_rs1() const{ throw 1; };
    virtual Register get_rs2() const{ throw 1; };
    virtual Register get_rd() const{throw 1; };
    virtual Register& get_rd_ref() { throw 1; };
    void execute();
    OperName get_name() const{ return name;}
    OperType get_type() const{ return type;}
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
    uint32_t get_f3() const{return funct3;}
    uint32_t get_f7() const{return funct7;}
    Register get_rs1() const{return rs1;};
    Register get_rs2() const{return rs2;};
    Register get_rd() const{ return rd; };
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
    uint32_t get_imm() const{return imm;}
    uint32_t get_f3() const{ return funct3; }
    Register get_rs1() const{ return rs1; };
    Register get_rd() const{ return rd; };
    Register& get_rd_ref() { return rd; };
    void set_rd(Register rd) { this->rd = rd; };
    int32_t get_load_addr() const{ return load_addres;}
    int32_t get_load_size() const{ return load_size;}
    void set_load_addr(int32_t l_add) {load_addres =l_add;}
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
    uint32_t get_imm() const { return imm; }
    uint32_t get_f3() const{ return funct3; }
    Register get_rs1() const{ return rs1; }
    Register get_rs2() const{ return rs2; }
    int32_t get_store_addr() const{ return store_addres;}
    int32_t get_store_size() const { return store_size;}
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
    bool br_is_taken =false;
    friend class Decoder;
    friend void Executors::MainInstrExecutorBGE(Oper *op );
    friend void Executors::MainInstrExecutorBGEU(Oper *op );
    friend void Executors::MainInstrExecutorBEQ(Oper *op );
    friend void Executors::MainInstrExecutorBNE(Oper *op );
    friend void Executors::MainInstrExecutorBLT(Oper *op );
    friend void Executors::MainInstrExecutorBLTU(Oper *op );
    
public:
    void calc_imm(uint32_t instr);
    uint32_t get_imm() const { return imm; }
    uint32_t get_f3() const { return funct3; }
    Register get_rs1() const { return rs1; }
    Register get_rs2() const { return rs2; }
    bool oper_br_is_taken() const{return br_is_taken;}
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
    Register get_rd() const{ return rd; };
    Register& get_rd_ref() { return rd; };
    void set_rd(Register rd){this->rd = rd;};
    uint32_t get_imm() const{ return imm; }
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
    uint32_t get_imm() const{ return imm; }
    Register get_rd() const{ return rd; };
    Register& get_rd_ref() { return rd; };
    void set_rd(Register rd) { this->rd = rd; };
    explicit OperJ(OperName name);
    OperJ()= default;
    ~OperJ()= default;
};



#endif