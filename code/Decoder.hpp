#ifndef MARLIN_DECODER_HPP
#define MARLIN_DECODER_HPP

#include "Oper.hpp"
#include "Log.hpp"
#include "Config.hpp"
#include "Executors.hpp"
#include "HazartUnit.hpp"
#include<vector>

class Regfile;



template <typename T>
class CyclededArray
{
private:
    uint64_t size = 0;
    uint64_t index = 0;
    std::vector<T> cont;
public:
    explicit CyclededArray(uint32_t size);
    ~CyclededArray() = default;
    T* get_next(){return &cont[index++%size]; }
};

template <typename T>
CyclededArray<T>::CyclededArray(uint32_t size) :size(size)
{
    if (size == 0)
        throw 1;
    cont.resize(size);
  
}


class Decoder
{
private:
    Config& config;
    HazartUnit& hazartUnit;
    uint32_t pipeline_size;
    OperName name =  OPER_NAME_NONE;
    OperType type = OPER_TYPE_NONE;
    AccessType mem_acc_type = ACCESS_TYPE_NONE;
    int32_t acc_size;
    CyclededArray<OperI> i_op_arr;
    CyclededArray<OperB> b_op_arr;
    CyclededArray<OperR> r_op_arr;
    CyclededArray<OperS> s_op_arr;
    CyclededArray<OperU> u_op_arr;
    CyclededArray<OperJ> j_op_arr;
    void (*executor)(Oper* , DE* ) =nullptr;
    uint32_t instr;
    void recognize_oper(uint32_t opcode, uint32_t funct3, uint32_t funct7);
    void print_and_raise_error(uint32_t instr);
public:
    explicit  Decoder(Config& config, HazartUnit& hz);
    ~Decoder() = default;
    Oper* decode32i(uint32_t instr, Regfile& reg);

};


#endif // !MARLIN_DECODER_HPP
