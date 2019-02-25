#ifndef MARLIN_DECODER_HPP
#define MARLIN_DECODER_HPP

#include "Oper.hpp"
#include "Log.hpp"
#include "Config.hpp"
class Regfile;
class Decoder
{
private:
    Config& config;
    OperName name =  OPER_NAME_NONE;
    OperType type = OPER_TYPE_NONE;
    void (*executor)(Oper* , DE* ) =nullptr;
    uint32_t instr;
    void recognize_oper(uint32_t opcode, uint32_t funct3, uint32_t funct7);
    void print_and_raise_error(uint32_t instr);
public:
    explicit  Decoder(Config& config):config(config){};
    ~Decoder(){};
    Oper* decode32i(uint32_t instr, Regfile& reg);

};


#endif // !MARLIN_DECODER_HPP
