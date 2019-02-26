#include "Executors.hpp"
void Executors::MainInstrExecutorAUIPC(Oper *op, DE *de)
{
    OperU *oper = static_cast<OperU *>(op);
    Register& rd = oper->get_rd();
    uint32_t imm = oper->get_imm();
    uint32_t val = imm+ de->pc;
    rd.set_value(val);
}
void Executors::MainInstrExecutorADDI(Oper *op, DE *de)
{
    (void*)de;
    OperI * oper = static_cast<OperI *>(op);
    uint32_t imm = oper->get_imm();
    Register rs1 = oper->get_rs1();
    Register& rd = oper->get_rd_ref();
    //Arithmetic overflow is ignored
    uint64_t res = static_cast<uint64_t>(rs1.get_value()) + imm;
    rd.set_value(static_cast<uint32_t>(res));
}

void Executors::MainInstrExecutorSLTI(Oper *op, DE *de)
{
    (void*)de;
    uint32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    uint32_t res;
    if (static_cast<int32_t>(rs1.get_value()) < static_cast<int32_t>(imm))
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    rd.set_value(static_cast<uint32_t>(res));
}


void Executors::MainInstrExecutorSLTIU(Oper *op, DE *de)
{
    (void*)de;
    uint32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    uint32_t res;
    if (static_cast<uint32_t>(rs1.get_value()) < static_cast<uint32_t>(imm))
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    rd.set_value(static_cast<uint32_t>(res));
}


void Executors::MainInstrExecutorANDI(Oper *op, DE *de)
{
    (void*)de;
    uint32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    uint32_t res;
    res = rs1.get_value() & imm;
    rd.set_value(res);
}
void Executors::MainInstrExecutorORI(Oper *op, DE *de)
{
    (void*)de;
    uint32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    uint32_t res;
    res = rs1.get_value() | imm;
    rd.set_value(res);
}
void Executors::MainInstrExecutorXORI(Oper *op, DE *de)
{
    (void*)de;
    uint32_t imm = op->get_imm();
    Register rs1 = op->get_rs1();
    Register& rd = op->get_rd_ref();
    uint32_t res;
    res = rs1.get_value() ^ imm;
    rd.set_value(res);
}

void Executors::MainInstrExecutorLUI(Oper *op, DE *de)
{
    (void*)de;
    OperU *oper = static_cast<OperU *>(op);
    Register& rd = oper->get_rd();
    uint32_t imm = oper->get_imm();
    uint32_t val = imm ;
    rd.set_value(val);
}


