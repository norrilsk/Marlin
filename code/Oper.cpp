#include "Oper.hpp"



Oper::Oper(OperName name, OperType type):type(type),name(name)
{
}
OperI::OperI(OperName name) :Oper(name, OPER_TYPE_I)
{

}

OperR::OperR(OperName name) :Oper(name, OPER_TYPE_R)
{

}

OperS::OperS(OperName name) :Oper(name, OPER_TYPE_S)
{

}

OperB::OperB(OperName name) :Oper(name, OPER_TYPE_B)
{

}
OperU::OperU(OperName name) :Oper(name, OPER_TYPE_U)
{

}
OperJ::OperJ(OperName name) :Oper(name, OPER_TYPE_J)
{

}