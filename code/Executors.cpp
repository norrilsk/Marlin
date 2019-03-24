#include "Oper.hpp"

void Executors::MainInstrExecutorLUI(Oper *op )
{
    
    OperU *oper = dynamic_cast<OperU *>(op);
    Register& rd = oper->get_rd_ref();
    uint32_t imm = oper->get_imm();
    uint32_t val = imm ;
    rd.set_value(val);
}
void Executors::MainInstrExecutorAUIPC(Oper *op )
{
    OperU *oper = dynamic_cast<OperU *>(op);
    Register& rd = oper->get_rd_ref();
    int32_t imm = oper->get_imm();
    int32_t val = imm+ op->get_pc();
    rd.set_value(val);
}
void Executors::MainInstrExecutorStore(Oper *op )
{
  
    OperS *oper = dynamic_cast<OperS*>(op);
    Register rs1 = oper->get_rs1();
    int32_t imm = oper->get_imm();
    int32_t addr = imm + rs1.get_value();
    oper->set_store_addr(addr);

}

void Executors::MainInstrExecutorLoad(Oper *op )
{

    OperI *oper = dynamic_cast<OperI*>(op);
    Register rs1 = oper->get_rs1();
    int32_t imm = oper->get_imm();
    int32_t addr = imm + rs1.get_value();
    oper->set_load_addr(addr);
    
}

void Executors::MainInstrExecutorADDI(Oper *op )
{
    
    OperI * oper = dynamic_cast<OperI *>(op);
    int32_t imm = oper->get_imm();
    Register rs1 = oper->get_rs1();
    Register& rd = oper->get_rd_ref();
    //Arithmetic overflow is ignored
    int64_t res = static_cast<int64_t>(rs1.get_value()) + imm;
    rd.set_value(static_cast<int32_t>(res));
}
void Executors::MainInstrExecutorSLTI(Oper *op )
{
    
    int32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    int32_t res;
    if (static_cast<int32_t>(rs1.get_value()) < static_cast<int32_t>(imm))
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    rd.set_value(static_cast<int32_t>(res));
}
void Executors::MainInstrExecutorSLTIU(Oper *op )
{
    
    int32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    int32_t res;
    if (static_cast<uint32_t>(rs1.get_value()) < static_cast<uint32_t>(imm))
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    rd.set_value(static_cast<int32_t>(res));
}
void Executors::MainInstrExecutorXORI(Oper *op )
{
    
    int32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    int32_t res;
    res = rs1.get_value() & imm;
    rd.set_value(res);
}
void Executors::MainInstrExecutorORI(Oper *op )
{
    
    int32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    int32_t res;
    res = rs1.get_value() | imm;
    rd.set_value(res);
}
void Executors::MainInstrExecutorANDI(Oper *op )
{
    
    int32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    int32_t res;
    res = rs1.get_value() ^ imm;
    rd.set_value(res);
}
void Executors::MainInstrExecutorSLLI(Oper *op )
{
    
    OperI *oper = dynamic_cast<OperI *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    uint32_t imm = oper->get_imm();
    uint32_t shift = imm & 0b11111;
    int32_t res;
    res = rs1.get_value() << shift;
    rd.set_value(res);
}
void Executors::MainInstrExecutorSRLI(Oper *op )
{
    
    OperI *oper = dynamic_cast<OperI *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    uint32_t imm = oper->get_imm();
    uint32_t shift = imm & 0b11111;
    int32_t res;
    res = static_cast<uint32_t >(rs1.get_value()) >> shift;
    rd.set_value(res);
}
void Executors::MainInstrExecutorSRAI(Oper *op )
{
    
    OperI *oper = dynamic_cast<OperI *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    uint32_t imm = oper->get_imm();
    uint32_t shift = imm & 0b11111;
    int32_t res;
    res = rs1.get_value() >> shift;
    rd.set_value(res);
}
void Executors::MainInstrExecutorADD(Oper *op )
{
    
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res = rs1.get_value() + rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorSUB(Oper *op )
{
    
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res = rs1.get_value() - rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorSLL(Oper *op )
{
    
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res = rs1.get_value() << rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorSLT(Oper *op )
{
    
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    uint32_t res;
    if(static_cast<int32_t>(rs1.get_value()) < static_cast<int32_t>(rs2.get_value()))
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    rd.set_value(static_cast<uint32_t>(res));
}
void Executors::MainInstrExecutorSLTU(Oper *op )
{
    
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    uint32_t res;
    if(static_cast<uint32_t>(rs1.get_value()) < static_cast<uint32_t>(rs2.get_value()))
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    rd.set_value(static_cast<uint32_t>(res));
}
void Executors::MainInstrExecutorXOR(Oper *op )
{
    
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res;
    res = rs1.get_value() ^ rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorSRL(Oper *op )
{
    
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res = static_cast<uint32_t >(rs1.get_value()) >> rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorSRA(Oper *op )
{
    
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res = rs1.get_value() >> rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorOR(Oper *op )
{
    
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res;
    res = rs1.get_value() | rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorAND(Oper *op )
{
    
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res;
    res = rs1.get_value() & rs2.get_value();
    rd.set_value(res);
}

void Executors::MainInstrExecutorBLTU(Oper *op )
{
    
    OperB *oper = dynamic_cast<OperB *>(op);
    uint32_t  rs1_val = static_cast<uint32_t >(oper->rs1.get_value());
    uint32_t  rs2_val = static_cast<uint32_t >(oper->rs2.get_value());
    bool res = false;
    if (rs1_val < rs2_val)
    {
        res = true;
    }
    int32_t j_addr = static_cast<int32_t>(oper->get_imm())+op->get_pc();
    oper-> set_br_target_addr(j_addr);
    oper->br_is_taken = res;
}

void Executors::MainInstrExecutorBGEU(Oper *op )
{
    
    OperB *oper = dynamic_cast<OperB *>(op);
    uint32_t  rs1_val = static_cast<uint32_t >(oper->rs1.get_value());
    uint32_t  rs2_val = static_cast<uint32_t >(oper->rs2.get_value());
    bool res = false;
    if (rs1_val >= rs2_val)
    {
        res = true;
    }
    int32_t j_addr = static_cast<int32_t>(oper->get_imm())+op->get_pc();
    oper-> set_br_target_addr(j_addr);
    oper->br_is_taken = res;
}


void Executors::MainInstrExecutorBEQ(Oper *op )
{
    
    OperB *oper = dynamic_cast<OperB *>(op);
    uint32_t  rs1_val = static_cast<uint32_t >(oper->rs1.get_value());
    uint32_t  rs2_val = static_cast<uint32_t >(oper->rs2.get_value());
    bool res = false;
    if (rs1_val == rs2_val)
    {
        res = true;
    }
    int32_t j_addr = static_cast<int32_t>(oper->get_imm())+op->get_pc();
    oper-> set_br_target_addr(j_addr);
    oper->br_is_taken = res;
}

void Executors::MainInstrExecutorBNE(Oper *op )
{
    
    OperB *oper = dynamic_cast<OperB *>(op);
    uint32_t  rs1_val = static_cast<uint32_t >(oper->rs1.get_value());
    uint32_t  rs2_val = static_cast<uint32_t >(oper->rs2.get_value());
    bool res = false;
    if (rs1_val != rs2_val)
    {
        res = true;
    }
    int32_t j_addr = static_cast<int32_t>(oper->get_imm())+op->get_pc();
    oper-> set_br_target_addr(j_addr);
    oper->br_is_taken = res;
}

void Executors::MainInstrExecutorBLT(Oper *op )
{
    
    OperB *oper = dynamic_cast<OperB *>(op);
    int32_t  rs1_val = static_cast<int32_t >(oper->rs1.get_value());
    int32_t  rs2_val = static_cast<int32_t >(oper->rs2.get_value());
    bool res = false;
    if (rs1_val < rs2_val)
    {
        res = true;
    }
    int32_t j_addr = static_cast<int32_t>(oper->get_imm())+op->get_pc();
    oper-> set_br_target_addr(j_addr);
    oper->br_is_taken = res;
}
void Executors::MainInstrExecutorBGE(Oper *op )
{
    
    OperB *oper = dynamic_cast<OperB *>(op);
    int32_t  rs1_val = static_cast<int32_t >(oper->rs1.get_value());
    int32_t  rs2_val = static_cast<int32_t >(oper->rs2.get_value());
    bool res = false;
    if (rs1_val >= rs2_val)
    {
        res = true;
    }
    int32_t j_addr = static_cast<int32_t>(oper->get_imm())+op->get_pc();
    oper-> set_br_target_addr(j_addr);
    oper->br_is_taken = res;
}


void Executors::MainInstrExecutorJAL(Oper *op)
{
    OperJ *oper = dynamic_cast<OperJ*>(op);
    int32_t j_addr = static_cast<int32_t>(oper->get_imm())+op->get_pc();
    oper-> set_br_target_addr(j_addr);
    
    Register& rd = oper->get_rd_ref();
    rd.set_value(op->get_pc()+4);
}
void Executors::MainInstrExecutorJALR(Oper *op )
{
    
    OperI *oper = dynamic_cast<OperI*>(op);
    Register rs1 = oper->get_rs1();
    int32_t j_addr = static_cast<int32_t>(oper->get_imm())+rs1.get_value();
    oper-> set_br_target_addr(j_addr);
    
    Register& rd = oper->get_rd_ref();
    rd.set_value(op->get_pc()+4);
}

void Executors::MainInstrExecutorECALL(Oper *op)
{
    //TODO: write something there;
    (void)op;
}