#include "Oper.hpp"

void Executors::MainInstrExecutorLUI(Oper *op, DE *de)
{
    (void)de;
    OperU *oper = dynamic_cast<OperU *>(op);
    Register& rd = oper->get_rd_ref();
    uint32_t imm = oper->get_imm();
    uint32_t val = imm ;
    rd.set_value(val);
}
void Executors::MainInstrExecutorAUIPC(Oper *op, DE *de)
{
    OperU *oper = dynamic_cast<OperU *>(op);
    Register& rd = oper->get_rd_ref();
    int32_t imm = oper->get_imm();
    int32_t val = imm+ de->pc;
    rd.set_value(val);
}
void Executors::MainInstrExecutorStore(Oper *op, DE *de)
{
    (void) de;
    OperS *oper = dynamic_cast<OperS*>(op);
    Register rs1 = oper->get_rs1();
    int32_t imm = oper->get_imm();
    int32_t addr = imm + rs1.get_value();
    oper->set_store_addr(addr);

}

void Executors::MainInstrExecutorLoad(Oper *op, DE *de)
{
    (void) de;
    OperI *oper = dynamic_cast<OperI*>(op);
    Register rs1 = oper->get_rs1();
    int32_t imm = oper->get_imm();
    int32_t addr = imm + rs1.get_value();
    oper->set_load_addr(addr);
    
}

void Executors::MainInstrExecutorADDI(Oper *op, DE *de)
{
    (void)de;
    OperI * oper = dynamic_cast<OperI *>(op);
    int32_t imm = oper->get_imm();
    Register rs1 = oper->get_rs1();
    Register& rd = oper->get_rd_ref();
    //Arithmetic overflow is ignored
    int64_t res = static_cast<int64_t>(rs1.get_value()) + imm;
    rd.set_value(static_cast<int32_t>(res));
}
void Executors::MainInstrExecutorSLTI(Oper *op, DE *de)
{
    (void)de;
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
void Executors::MainInstrExecutorSLTIU(Oper *op, DE *de)
{
    (void)de;
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
void Executors::MainInstrExecutorXORI(Oper *op, DE *de)
{
    (void)de;
    int32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    int32_t res;
    res = rs1.get_value() & imm;
    rd.set_value(res);
}
void Executors::MainInstrExecutorORI(Oper *op, DE *de)
{
    (void)de;
    int32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    int32_t res;
    res = rs1.get_value() | imm;
    rd.set_value(res);
}
void Executors::MainInstrExecutorANDI(Oper *op, DE *de)
{
    (void)de;
    int32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    int32_t res;
    res = rs1.get_value() ^ imm;
    rd.set_value(res);
}
void Executors::MainInstrExecutorSLLI(Oper *op, DE *de)
{
    (void)de;
    OperI *oper = dynamic_cast<OperI *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    uint32_t imm = oper->get_imm();
    uint32_t shift = imm & 0b11111;
    int32_t res;
    res = rs1.get_value() << shift;
    rd.set_value(res);
}
void Executors::MainInstrExecutorSRLI(Oper *op, DE *de)
{
    (void)de;
    OperI *oper = dynamic_cast<OperI *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    uint32_t imm = oper->get_imm();
    uint32_t shift = imm & 0b11111;
    int32_t res;
    res = static_cast<uint32_t >(rs1.get_value()) >> shift;
    rd.set_value(res);
}
void Executors::MainInstrExecutorSRAI(Oper *op, DE *de)
{
    (void)de;
    OperI *oper = dynamic_cast<OperI *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    uint32_t imm = oper->get_imm();
    uint32_t shift = imm & 0b11111;
    int32_t res;
    res = rs1.get_value() >> shift;
    rd.set_value(res);
}
void Executors::MainInstrExecutorADD(Oper *op, DE *de)
{
    (void)de;
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res = rs1.get_value() + rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorSUB(Oper *op, DE *de)
{
    (void)de;
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res = rs1.get_value() - rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorSLL(Oper *op, DE *de)
{
    (void)de;
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res = rs1.get_value() << rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorSLT(Oper *op, DE *de)
{
    (void)de;
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
void Executors::MainInstrExecutorSLTU(Oper *op, DE *de)
{
    (void)de;
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
void Executors::MainInstrExecutorXOR(Oper *op, DE *de)
{
    (void)de;
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res;
    res = rs1.get_value() ^ rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorSRL(Oper *op, DE *de)
{
    (void)de;
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res = static_cast<uint32_t >(rs1.get_value()) >> rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorSRA(Oper *op, DE *de)
{
    (void)de;
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res = rs1.get_value() >> rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorOR(Oper *op, DE *de)
{
    (void)de;
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res;
    res = rs1.get_value() | rs2.get_value();
    rd.set_value(res);
}
void Executors::MainInstrExecutorAND(Oper *op, DE *de)
{
    (void)de;
    OperR *oper = dynamic_cast<OperR *>(op);
    Register& rd = oper->get_rd_ref();
    Register rs1 = op->get_rs1();
    Register rs2 = op->get_rs2();
    int32_t res;
    res = rs1.get_value() & rs2.get_value();
    rd.set_value(res);
}

void Executors::MainInstrExecutorBLTU(Oper *op, DE *de)
{
    (void)de;
    OperB *oper = dynamic_cast<OperB *>(op);
    uint32_t  rs1_val = static_cast<uint32_t >(oper->rs1.get_value());
    uint32_t  rs2_val = static_cast<uint32_t >(oper->rs2.get_value());
    bool res = false;
    if (rs1_val < rs2_val)
    {
        res = true;
    }
    oper->br_is_taken = res;
}
void Executors::MainInstrExecutorJAL(Oper *op, DE *de)
{
    OperJ *oper = dynamic_cast<OperJ*>(op);
   // int32_t j_addr = static_cast<int32_t>(oper->get_imm())+de->pc;
    Register& rd = oper->get_rd_ref();
    rd.set_value(de->pc+4);
}