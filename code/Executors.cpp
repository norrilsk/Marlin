#include "Executors.hpp"
void Executors::MainInstrExecutorAUIPC(Oper *op, DE *de)
{
    OperU *oper = static_cast<OperU *>(op);
    Register rd = oper->get_rd();
    uint32_t imm = oper->get_imm();
    rd = imm + de->pc;
    oper->set_rd(rd);
}
