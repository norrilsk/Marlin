#ifndef MARLIN_EXECUTORS_HPP
#define MARLIN_EXECUTORS_HPP
#include "Oper.hpp"
namespace Executors
{
  void MainInstrExecutorAUIPC(Oper *op, DE *de);
  void MainInstrExecutorLUI(Oper *op, DE *de);
  void MainInstrExecutorADDI(Oper *op, DE *de);
  void MainInstrExecutorSLTI(Oper *op, DE *de);
  void MainInstrExecutorSLTIU(Oper *op, DE *de);
  void MainInstrExecutorANDI(Oper *op, DE *de);
  void MainInstrExecutorORI(Oper *op, DE *de);
  void MainInstrExecutorXORI(Oper *op, DE *de);
  void MainInstrExecutorSW(Oper *op, DE *de);
}

#endif //MARLIN_EXECUTORS_HPP
