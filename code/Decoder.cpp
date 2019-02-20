#include"Decoder.hpp"
Oper* Decoder::decode32i(uint32_t instr)
{
    this->instr = instr;
    Oper* op;
    OperI* op_i;
    OperB* op_b;
    OperR* op_r;
    OperS* op_s;
    OperU* op_u;
    OperJ* op_j;
    name = OPER_NAME_NONE;
    type=  OPER_TYPE_NONE;
    uint32_t funct3 = (instr >> 12) & 0b0111;
    uint32_t funct7 = (instr >> 25) &0b01111111;
    uint32_t opcode = instr & 0b1111111;
    find_name_and_type(opcode, funct3, funct7);
    switch (type)
    {
    case OPER_TYPE_R:
        op_r = new OperR(name);
        op = dynamic_cast<Oper*>(op_r);
        break;
    case OPER_TYPE_I:
        op_i = new OperI(name);
        op = dynamic_cast<Oper*>(op_i);
        break;
    case OPER_TYPE_S:
        op_s = new OperS(name);
        op = dynamic_cast<Oper*>(op_s);
        break;
    case OPER_TYPE_B:
        op_b = new OperB(name);
        op = dynamic_cast<Oper*>(op_b);
        break;
    case OPER_TYPE_U:
        op_u = new OperU(name);
        op = dynamic_cast<Oper*>(op_u);
        break;
    case OPER_TYPE_J:
        op_j = new OperJ(name);
        op = dynamic_cast<Oper*>(op_j);
        break;
    default:
        print_and_raise_error(instr);
    }

    op->calc_imm(instr);

    return op;
}
void Decoder::find_name_and_type(uint32_t opcode, uint32_t funct3, uint32_t funct7)
{
    switch (opcode)
    {
        //RTYPE
    case 0b0110111://LUI
        name = OPER_NAME_LUI;
        type = OPER_TYPE_R;
        break;
    case 0b0010111://AUIPC
        name = OPER_NAME_AUIPC;
        type = OPER_TYPE_R;
        break;
    case 0b1101111:
        name = OPER_NAME_JAL;
        type = OPER_TYPE_J;
        break;
    case 0b1100111:
        name = OPER_NAME_JALR;
        if (funct3 != 0)
            print_and_raise_error(instr);
        type = OPER_TYPE_I;
        break;
    case 0b1100011:
        type = OPER_TYPE_B;
        switch (funct3)
        {
        case 0b000:
            name = OPER_NAME_BEQ;
            break;
        case 0b001:
            name = OPER_NAME_BNE;
            break;
        case 0b100:
            name = OPER_NAME_BLT;
            break;
        case 0b101:
            name = OPER_NAME_BGE;
            break;
        case 0b110:
            name = OPER_NAME_BLTU;
            break;
        case 0b111:
            name = OPER_NAME_BGEU;
            break;
        default:
            print_and_raise_error(instr);
        }
        break;
    case 0b0000011:
        type = OPER_TYPE_I;
        switch (funct3)
        {
        case 0b000:
            name = OPER_NAME_LB;
            break;
        case 0b001:
            name = OPER_NAME_LH;
            break;
        case 0b010:
            name = OPER_NAME_LW;
            break;
        case 0b100:
            name = OPER_NAME_LBU;
            break;
        case 0b101:
            name = OPER_NAME_LHU;
            break;
        default:
            print_and_raise_error(instr);
        }
        break;
    case 0b0100011:
        type = OPER_TYPE_S;
        switch (funct3)
        {
        case 0b000:
            name = OPER_NAME_SB;
            break;
        case 0b001:
            name = OPER_NAME_SH;
            break;
        case 0b010:
            name = OPER_NAME_SW;
            break;
        default:
            print_and_raise_error(instr);
        }
        break;
    case  0b0010011:
        switch (funct3)
        {
        case 0b001:
        case 0b101: //TODO: add this later
            print_and_raise_error(instr);
        case 0b000:
            type = OPER_TYPE_I;
            name = OPER_NAME_ADDI;
            break;
        case 0b010:
            type = OPER_TYPE_I;
            name = OPER_NAME_SLTI;
            break;
        case 0b011:
            type = OPER_TYPE_I;
            name = OPER_NAME_SLTIU;
            break;
        case 0b100:
            type = OPER_TYPE_I;
            name = OPER_NAME_XORI;
            break;
        case 0b110:
            type = OPER_TYPE_I;
            name = OPER_NAME_ORI;
            break;
        case 0b111:
            type = OPER_TYPE_I;
            name = OPER_NAME_ANDI;
            break;
        default:
            print_and_raise_error(instr);
        }
        break;
    case 0b0110011:
        type = OPER_TYPE_R;
        switch (funct3)
        {
        case 0b000:
            switch (funct7)
            {
            case 0b0000000:
                name = OPER_NAME_ADD;
                break;
            case 0b0100000:
                name = OPER_NAME_SUB;
                break;
            default:
                print_and_raise_error(instr);
            }
            break;
        case 0b001:
            switch (funct7)
            {
            case 0b0000000:
                name = OPER_NAME_SLL;
                break;
            default:
                print_and_raise_error(instr);
            }
            break;
        case 0b010:
            switch (funct7)
            {
            case 0b0000000:
                name = OPER_NAME_SLT;
                break;
            default:
                print_and_raise_error(instr);
            }
            break;
        case 0b011:
            switch (funct7)
            {
            case 0b0000000:
                name = OPER_NAME_SLTU;
                break;
            default:
                print_and_raise_error(instr);
            }
            break;
        case 0b100:
            switch (funct7)
            {
            case 0b0000000:
                name = OPER_NAME_XOR;
                break;
            default:
                print_and_raise_error(instr);
            }
            break;
        case 0b101:
            switch (funct7)
            {
            case 0b0000000:
                name = OPER_NAME_SRL;
                break;
            case 0b0100000:
                name = OPER_NAME_SRA;
                break;
            default:
                print_and_raise_error(instr);
            }
            break;
        case 0b110:
            switch (funct7)
            {
            case 0b0000000:
                name = OPER_NAME_OR;
                break;
            default:
                print_and_raise_error(instr);
            }
            break;
        case 0b111:
            switch (funct7)
            {
            case 0b0000000:
                name = OPER_NAME_AND;
                break;
            default:
                print_and_raise_error(instr);
            }
            break;
        default:
            print_and_raise_error(instr);
        }
        break;
    default:
        print_and_raise_error(instr);

    }

}
void Decoder::print_and_raise_error(uint32_t instr)
{
    Log::Loger& log  = config.get_log_ref();
    log << Log::err << "Decoder error while decoding " << std::to_string(instr) <<Log::endl;
    throw -1;
}