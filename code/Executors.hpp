#ifndef MARLIN_EXECUTORS_HPP
#define MARLIN_EXECUTORS_HPP
#include "Oper.hpp"
namespace Executors
{
  void MainInstrExecutorLUI(Oper *op, DE *de);
  void MainInstrExecutorAUIPC(Oper *op, DE *de);
  //void MainInstrExecutorJAL(Oper *op, DE *de);
  //void MainInstrExecutorJALR(Oper *op, DE *de);
  //void MainInstrExecutorBEQ(Oper *op, DE *de);
  //void MainInstrExecutorBNE(Oper *op, DE *de);
  //void MainInstrExecutorBLT(Oper *op, DE *de);
  //void MainInstrExecutorBGE(Oper *op, DE *de);
  //void MainInstrExecutorBLTU(Oper *op, DE *de);
  //void MainInstrExecutorBGEU(Oper *op, DE *de);
  void MainInstrExecutorStoreLoad(Oper *op, DE *de); //SW SH SB LW LH LHU LB LBU
  void MainInstrExecutorADDI(Oper *op, DE *de);
  void MainInstrExecutorSLTI(Oper *op, DE *de);
  void MainInstrExecutorSLTIU(Oper *op, DE *de);
  void MainInstrExecutorXORI(Oper *op, DE *de);
  void MainInstrExecutorORI(Oper *op, DE *de);
  void MainInstrExecutorANDI(Oper *op, DE *de);
  void MainInstrExecutorSLLI(Oper *op, DE *de);
  void MainInstrExecutorSRLI(Oper *op, DE *de);
  void MainInstrExecutorSRAI(Oper *op, DE *de);
  void MainInstrExecutorADD(Oper *op, DE *de);
  void MainInstrExecutorSUB(Oper *op, DE *de);
  void MainInstrExecutorSLL(Oper *op, DE *de);
  void MainInstrExecutorSLT(Oper *op, DE *de);
  void MainInstrExecutorSLTU(Oper *op, DE *de);
  void MainInstrExecutorXOR(Oper *op, DE *de);
  void MainInstrExecutorSRL(Oper *op, DE *de);
  void MainInstrExecutorSRA(Oper *op, DE *de);
  void MainInstrExecutorOR(Oper *op, DE *de);
  void MainInstrExecutorAND(Oper *op, DE *de);
}

#endif //MARLIN_EXECUTORS_HPP
