#include"Decoder.hpp"
#include "Marlin.hpp"

Decoder::Decoder(Config& config, HazartUnit& hz) :config(config), hazartUnit(hz) , pipeline_size(config.get_num_of_pipeline_stages()),
                                i_op_arr(pipeline_size), b_op_arr(pipeline_size), r_op_arr(pipeline_size), s_op_arr(pipeline_size), u_op_arr(pipeline_size), j_op_arr(pipeline_size)
{

};

Oper* Decoder::decode32i(uint32_t instr, Regfile& reg)
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
    mem_acc_type = ACCESS_TYPE_NONE;
    executor = nullptr;
    acc_size = 0;
    uint32_t funct3 = (instr >> 12) & 0b0111u;
    uint32_t funct7 = (instr >> 25) &0b01111111u;
    uint32_t opcode = instr & 0b1111111u;
    uint32_t num_rs1 =(instr >> 15)&0b011111u;
    uint32_t num_rs2 = (instr >> 20) &0b011111u;
    uint32_t num_rd = (instr >> 7) &0b011111u;
    recognize_oper(opcode, funct3, funct7);
   
    switch (type)
    {
    case OPER_TYPE_R:
        op_r = r_op_arr.get_next();
        
        if (reg.is_dirty(num_rs1))
        {
            op_r->rs1  = hazartUnit.hazart_in_decode(reg.get_reg(num_rs1));
        }
        else
        {
            op_r->rs1 = reg.get_reg(num_rs1);
        }
        
        if (reg.is_dirty(num_rs2))
        {
            op_r->rs2 = hazartUnit.hazart_in_decode(reg.get_reg(num_rs2));
        }
        else
        {
            op_r->rs2 = reg.get_reg(num_rs2);
        }
        op_r->rd =reg.get_reg(num_rd,ACCESS_TYPE_WRITE);
        op = dynamic_cast<Oper*>(op_r);
        break;
    case OPER_TYPE_I:
        op_i = i_op_arr.get_next();
        op = dynamic_cast<Oper*>(op_i);
        
        if (reg.is_dirty(num_rs1))
        {
            op_i->rs1 = hazartUnit.hazart_in_decode(reg.get_reg(num_rs1));
        }
        else
        {
            op_i->rs1 = reg.get_reg(num_rs1);
        }
        op_i->rd =reg.get_reg(num_rd,ACCESS_TYPE_WRITE);
        break;
    case OPER_TYPE_S:
        op_s = s_op_arr.get_next();
        op = dynamic_cast<Oper*>(op_s);
        if (reg.is_dirty(num_rs1))
        {
            op_s->rs1 = hazartUnit.hazart_in_decode(reg.get_reg(num_rs1));
        }
        else
        {
            op_s->rs1 = reg.get_reg(num_rs1);
        }
        if (reg.is_dirty(num_rs2))
        {
            op_s->rs2 = hazartUnit.hazart_in_decode(reg.get_reg(num_rs2));
        }
        else
        {
            op_s->rs2 = reg.get_reg(num_rs2);
        }
        break;
    case OPER_TYPE_B:
        op_b = b_op_arr.get_next();
        op = dynamic_cast<Oper*>(op_b);
        if (reg.is_dirty(num_rs1))
        {
            op_b->rs1 = hazartUnit.hazart_in_decode(reg.get_reg(num_rs1));
        }
        else
        {
            op_b->rs1 = reg.get_reg(num_rs1);
        }
        if (reg.is_dirty(num_rs2))
        {
            op_b->rs2 = hazartUnit.hazart_in_decode(reg.get_reg(num_rs2));
        }
        else
        {
            op_b->rs2 = reg.get_reg(num_rs2);
        }
        break;
    case OPER_TYPE_U:
        op_u = u_op_arr.get_next();
        op = dynamic_cast<Oper*>(op_u);
        op_u->rd = reg.get_reg(num_rd,ACCESS_TYPE_WRITE);
        break;
    case OPER_TYPE_J:
        op_j = j_op_arr.get_next();
        op = dynamic_cast<Oper*>(op_j);
        op_j->rd = reg.get_reg(num_rd,ACCESS_TYPE_WRITE);
        break;
    default:
        op = new Oper;
        print_and_raise_error(instr);
    }
    op->name = this->name;
    op->type = this->type;
    op->main_executor = executor; // set function for execution
    op->opcode = opcode;
    op->calc_imm(instr); //may be it should be moved to execute stage

    return op;
}
void Decoder::recognize_oper(uint32_t opcode, uint32_t funct3, uint32_t funct7)
{
    switch (opcode)
    {
        //RTYPE
    case 0b0110111://LUI
        name = OPER_NAME_LUI;
        type = OPER_TYPE_U;
        executor = &(Executors::MainInstrExecutorLUI);
        break;
    case 0b0010111://AUIPC
        name = OPER_NAME_AUIPC;
        type = OPER_TYPE_U;
        executor = &(Executors::MainInstrExecutorAUIPC);
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
            mem_acc_type = ACCESS_TYPE_READ;
            acc_size =1;
            break;
        case 0b001:
            name = OPER_NAME_LH;
            mem_acc_type = ACCESS_TYPE_READ;
            acc_size =2;
            break;
        case 0b010:
            name = OPER_NAME_LW;
            mem_acc_type = ACCESS_TYPE_READ;
            acc_size =4;
            break;
        case 0b100:
            name = OPER_NAME_LBU;
            mem_acc_type = ACCESS_TYPE_READ;
            acc_size = 1;
            break;
        case 0b101:
            name = OPER_NAME_LHU;
            mem_acc_type = ACCESS_TYPE_READ;
            acc_size = 2;
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
            mem_acc_type = ACCESS_TYPE_WRITE;
            acc_size = 1;
            break;
        case 0b001:
            name = OPER_NAME_SH;
            mem_acc_type = ACCESS_TYPE_WRITE;
            acc_size = 2;
            break;
        case 0b010:
            name = OPER_NAME_SW;
            mem_acc_type = ACCESS_TYPE_WRITE;
            acc_size = 4;
            executor = &(Executors::MainInstrExecutorSW);
            break;
        default:
            print_and_raise_error(instr);
        }
        break;
    case  0b0010011:
        switch (funct3)
        {
        case 0b001: //SLLI
        case 0b101: //SRLI SRAI TODO: add this later
            print_and_raise_error(instr);
        case 0b000:
            type = OPER_TYPE_I;
            name = OPER_NAME_ADDI;
            executor = &(Executors::MainInstrExecutorADDI);
            break;
        case 0b010:
            type = OPER_TYPE_I;
            name = OPER_NAME_SLTI;
            executor = &(Executors::MainInstrExecutorSLTI);
            break;
        case 0b011:
            type = OPER_TYPE_I;
            name = OPER_NAME_SLTIU;
            executor = &(Executors::MainInstrExecutorSLTIU);
            break;
        case 0b100:
            type = OPER_TYPE_I;
            name = OPER_NAME_XORI;
            executor = &(Executors::MainInstrExecutorXORI);
            break;
        case 0b110:
            type = OPER_TYPE_I;
            name = OPER_NAME_ORI;
            executor = &(Executors::MainInstrExecutorORI);
            break;
        case 0b111:
            type = OPER_TYPE_I;
            name = OPER_NAME_ANDI;
            executor = &(Executors::MainInstrExecutorANDI);
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