#include "Executors.hpp"
void Executors::MainInstrExecutorAUIPC(Oper *op, DE *de)
{
    OperU *oper = dynamic_cast<OperU *>(op);
    Register& rd = oper->get_rd_ref();
    int32_t imm = oper->get_imm();
    int32_t val = imm+ de->pc;
    rd.set_value(val);
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


void Executors::MainInstrExecutorANDI(Oper *op, DE *de)
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
void Executors::MainInstrExecutorXORI(Oper *op, DE *de)
{
    (void)de;
    int32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    int32_t res;
    res = rs1.get_value() ^ imm;
    rd.set_value(res);
}

void Executors::MainInstrExecutorLUI(Oper *op, DE *de)
{
    (void)de;
    OperU *oper = dynamic_cast<OperU *>(op);
    Register& rd = oper->get_rd_ref();
    int32_t imm = oper->get_imm();
    int32_t val = imm ;
    rd.set_value(val);
}

void Executors::MainInstrExecutorSW(Oper *op, DE *de)
{
    (void) de;
    OperS *oper = dynamic_cast<OperS*>(op);
    Register rs1 = oper->get_rs1();
    int32_t imm = oper->get_imm();
    int32_t addr = imm+ rs1.get_value();
    oper->set_store_addr(addr);
    
}