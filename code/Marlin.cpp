#include "Marlin.hpp"
#include<iostream>

Marlin::Marlin(std::string path_to_data, std::string path_to_conf ): config(path_to_conf), log(config.get_log_ref()),
                   trace(config.get_trace_ref()), mmu(config), regfile(config),
                   hazartUnit(config, regfile, fd_cell,de_cell,em_cell,mw_cell,fetch_cell), decoder(config,hazartUnit),
                   is_dump_trace(config.is_dump_trace())
{
    
    ElfMarlin elf(path_to_data.c_str(),  config.get_log_marlin());
   
    op_mode = static_cast<uint64_t >(elf.get_i_class());
    if (op_mode != 1)
    {
        std::cerr<<"wrong operation mode(64 bit unsupported yet)"<<std::endl;
        throw 99;
    }
    elf.load(mmu);
    
    //we do not have OS, so we have to initialize SP
    Register sp =regfile.get_reg(REGISTER_NAME_SP, ACCESS_TYPE_WRITE);
    sp.set_value(config.get_mem_size() - 5);
    
    // TODO: we have to initialize sp pointing to argc , argv
    uint32_t argc = 0;
    mmu.write_to_mem(&argc, sp.get_value(),4);
    regfile.write_reg(sp);
    
    
    WF* wf = fetch_cell.get_store_ptr();
    wf->pc = static_cast<uint32_t >(elf.get_entry32());
    wf->is_stall = false;
    fetch_cell.update();
}

void Marlin::run()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    
    while(!is_stop)
    {
        write_back();
        memory_access();
        execute();
        decode();
        fetch();
        
        fetch_cell.update();
        fd_cell.update();
        de_cell.update();
        em_cell.update();
        mw_cell.update();
        clocks++;
    }
    end = std::chrono::system_clock::now();
    
    execution_time = std::chrono::duration_cast<std::chrono::nanoseconds>
        (end-start).count();

    


    if (this->config.get_log_marlin())
    {
        dump_stat();
    }
    
}

void Marlin::fetch()
{
    WF* wf = fetch_cell.get_load_ptr();
    WF* wf_store = fetch_cell.get_store_ptr();
    FD* fd = fd_cell.get_store_ptr();
    uint32_t  instr;
    fd->is_stall =wf->is_stall || wf->is_hazard_stall;
    if (fd->is_stall || fd_cell.is_stop())
    {
        return;
    }
    uint32_t  pc = wf->is_jump? wf->pc_jump :wf->pc;
    mmu.read_from_mem(&instr,pc,4);
    
    fd->pc = static_cast<uint32_t>(pc);
    fd->instr = instr;
    fd->is_stall = false;
    wf_store->pc = pc +4;
}
void Marlin::decode()
{
    FD* fd = fd_cell.get_load_ptr();
    DE* de = de_cell.get_store_ptr();
    de->is_stall = fd->is_stall || fd->is_hazard_stall;
    if(de->is_stall || de_cell.is_stop())
    {
        return;
    }
    Oper* op;
    op = decoder.decode32i(fd->instr, regfile);
    op->set_pc(fd->pc);
    if (de_cell.is_stop())
    {
        //it means that registers are not ready now, we need to wait
        // and fix regfile
        hazartUnit.fix_dirtness(op);
    }
    de->op = op;
    de->pc = fd->pc;
}
void Marlin::execute()
{
    DE* de = de_cell.get_load_ptr();
    EM* em = em_cell.get_store_ptr();
    em->is_stall = de->is_stall || de->is_hazard_stall;
    Oper* oper = de->op;
    if(em->is_stall || em_cell.is_stop())
    {
        if (!de->is_stall  && de->is_hazard_stall)
            hazartUnit.fix_dirtness(oper);
        return;
    }
    oper->execute();
    if ( oper->is_oper_branch() &&
        (OPER_TYPE_J == oper->get_type() ||
        OPER_TYPE_I == oper->get_type()  ||
            (OPER_TYPE_B == oper->get_type() && dynamic_cast<OperB*>(oper)->oper_br_is_taken())))
    {
            hazartUnit.branch_hazart(oper);
    }
    
    if (OPER_TYPE_SYSTEM == oper->get_type())
    {
        hazartUnit.exit_call(oper);
    }
    
    em->pc = de->pc;
    em->op = de->op;
    em->is_stall = de->is_stall;
}
void Marlin::memory_access()
{
    EM* em = em_cell.get_load_ptr();
    MW* mw = mw_cell.get_store_ptr();
    Oper* oper = em->op;
    mw->is_stall = em->is_stall || em->is_hazard_stall;
    if (mw->is_stall || mw_cell.is_stop())
    {
        if (!em->is_stall  && em->is_hazard_stall)
            hazartUnit.fix_dirtness(oper);
        return;
    }
    AccessType op_acc_type = oper->get_mem_acc_type();
    OperType  op_type = oper->get_type();
    if ((ACCESS_TYPE_WRITE == op_acc_type )&&(OPER_TYPE_S == op_type))
    {
        OperS* op_s = dynamic_cast<OperS*>(oper);
        int32_t data = op_s->get_rs2().get_value();
        uint64_t addr = static_cast<uint64_t>(op_s->get_store_addr());
        uint64_t size = static_cast<uint64_t>(op_s->get_store_size());
        mmu.write_to_mem(&data,addr,size);
    }
    if ((ACCESS_TYPE_READ == op_acc_type) &&(OPER_TYPE_I == op_type))
    {
        OperI* op_i = dynamic_cast<OperI*>(oper);
        int32_t data = 0;
        uint64_t addr = static_cast<uint64_t>(op_i->get_load_addr());
        uint64_t size = static_cast<uint64_t>(op_i->get_load_size());
        mmu.read_from_mem(&data,addr,size);
        OperName name = op_i->get_name();
        if (OPER_NAME_LHU == name
            || OPER_NAME_LBU ==name)
        {
            data = sign_extend(data, size, EXTEND_TYPE_ZERO);
        }
        else if ( OPER_NAME_LH == name
                || OPER_NAME_LB == name
                || OPER_NAME_LW == name
                )
        {
            data = sign_extend(data, size, EXTEND_TYPE_SIGN);
        }
        else
            throw 1;
    
        oper->get_rd_ref().set_value(data);
    }
    
    mw->op = em->op;
    mw->pc = em->pc;
}

void Marlin::write_back()
{
    MW* mw = mw_cell.get_load_ptr();
    Oper* oper = mw->op;
    if (mw->is_stall || mw->is_hazard_stall)
    {
        if (!mw->is_stall  && mw->is_hazard_stall)
            hazartUnit.fix_dirtness(oper);
        return;
    }
    if (oper->get_type() == OPER_TYPE_R
        || oper->get_type() == OPER_TYPE_I
        || oper->get_type()== OPER_TYPE_U
        || oper->get_type() == OPER_TYPE_J
        )
    {
        regfile.write_reg(oper->get_rd());
    }
    ic++;
    if (is_dump_trace)
    {
        dump_instruction(oper);
    }
    if (OPER_NAME_ECALL == oper->get_name())
        is_stop = true;
}


int32_t Marlin::sign_extend(int32_t n, int32_t size, ExtendType extend_type)
{
    if (4 == size)
        return n;
    uint32_t  num  = static_cast<uint32_t >(n);
    if (EXTEND_TYPE_ZERO == extend_type)
    {
        uint32_t mask = static_cast<uint32_t >((0b1 << size) - 1);
        return static_cast<int32_t >(mask & num);
    }
    else if ( EXTEND_TYPE_SIGN == extend_type)
    {
        uint32_t sign_bit = static_cast<uint32_t >(num >> (size - 1)) &0b1;
        uint32_t mask = static_cast<uint32_t >((0b1 << size) - 1);
        if ( sign_bit)
        {
            return static_cast<int32_t >(~mask | num);
        }
        else
        {
            return static_cast<int32_t >(mask & num);
        }
    }
    else
        throw 1;
}


std::string Marlin::intToHex( int32_t i )
{
    std::stringstream stream;
    stream << "0x"
           << std::setfill ('0') << std::setw(sizeof(int32_t)*2)
           << std::hex << i;
    return stream.str();
}


void Marlin::dump_stat()
{
    log << std::string("ic : ")<<std::to_string(this->ic)<<Log::endl;
    log << std::string("clocks : ")<< std::to_string(this->clocks)<<Log::endl;
    log << std::string("execution time :")<<std::to_string(execution_time)<<std::string("ns")<<Log::endl;
    log << std::string("KIPS : ") << std::to_string(static_cast<double>(ic*1000000)/(execution_time)) << Log::endl;
    hazartUnit.dump_stat();
}

void Marlin::dump_instruction(Oper *op)
{
    //trace << std::to_string(op->get_name())<< " "<< std::to_string(op->get_pc()) << " " << Log::endl;
    (void)op;
    
    trace << std::to_string(op->get_pc()) <<Log::endl;
    //TODO: Напишите код здесь
    std::string str_pc(12 - intToHex(op->get_pc()).length(), ' ');
    std::string str_name(12 - oper_name(op->get_name()).length(), ' ');
    trace << intToHex(op->get_pc()) << str_pc << oper_name(op->get_name()) << str_name;
    if ((op->get_type() == OPER_TYPE_R) || (op->get_type() == OPER_TYPE_I)|| (op->get_type() == OPER_TYPE_S)|| (op->get_type() == OPER_TYPE_B))
    {
        std::string str_rs1(12 - reg_name(op->get_rs1().get_name()).length(), ' ');
        trace << "rs1 " << reg_name(op->get_rs1().get_name())<< str_rs1;
    }
    else
    {
        std::string tmp(16, ' ');
        trace << tmp;
    }
    if ((op->get_type() == OPER_TYPE_R)|| (op->get_type() == OPER_TYPE_S)|| (op->get_type() == OPER_TYPE_B))
    {
        std::string str_rs2(12 - reg_name(op->get_rs2().get_name()).length(), ' ');
        trace << "rs2 "<< reg_name(op->get_rs2().get_name())<< str_rs2;
    }
    else
    {
        std::string tmp(16, ' ');
        trace << tmp;
    }
    if ((op->get_type() == OPER_TYPE_R)|| (op->get_type() == OPER_TYPE_I)|| (op->get_type() == OPER_TYPE_U)|| (op->get_type() == OPER_TYPE_J))
    {
        std::string str_rd(12 - reg_name(op->get_rd().get_name()).length(), ' ');
        trace << "rd "<<reg_name(op->get_rd().get_name())<< str_rd;
    }
    else
    {
        std::string tmp(15, ' ');
        trace << tmp;
    }
    if ((op->get_type() == OPER_TYPE_I)|| (op->get_type() == OPER_TYPE_S)|| (op->get_type() == OPER_TYPE_B)|| (op->get_type() == OPER_TYPE_U)|| (op->get_type() == OPER_TYPE_J))
    {
        std::string str_imm(12 - intToHex(op->get_imm()).length(), ' ');
        trace << "imm "<< intToHex(static_cast<int32_t>(op->get_imm()))<< str_imm;
    }
    else
    {
        std::string tmp(16, ' ');
        trace << tmp;
    }
    trace << Log::endl;
}

std::string Marlin::reg_name(RegName name)
{
    std::string s_name = " no name ";

    switch (name) {
        case REGISTER_NAME_ZERO:
            s_name = "zero";
            break;
        case REGISTER_NAME_RA:
            s_name = "ra";
            break;
        case REGISTER_NAME_SP:
            s_name = "sp";
            break;
        case REGISTER_NAME_GP:
            s_name = "gp";
            break;
        case REGISTER_NAME_TP:
            s_name = "tp";
            break;
        case REGISTER_NAME_T0:
            s_name = "t0";
            break;
        case REGISTER_NAME_T1:
            s_name = "t1";
            break;
        case REGISTER_NAME_T2:
            s_name = "t2";
            break;
        case REGISTER_NAME_S0:
            s_name = "s0";
            break;
        case REGISTER_NAME_S1:
            s_name = "s1";
            break;
        case REGISTER_NAME_A0:
            s_name = "a0";
            break;
        case REGISTER_NAME_A1:
            s_name = "a1";
            break;
        case REGISTER_NAME_A2:
            s_name = "a2";
            break;
        case REGISTER_NAME_A3:
            s_name = "a3";
            break;
        case REGISTER_NAME_A4:
            s_name = "a4";
            break;
        case REGISTER_NAME_A5:
            s_name = "a5";
            break;
        case REGISTER_NAME_A6:
            s_name = "a6";
            break;
        case REGISTER_NAME_A7:
            s_name = "a7";
            break;
        case REGISTER_NAME_S2:
            s_name = "s2";
            break;
        case REGISTER_NAME_S3:
            s_name = "s3";
            break;
        case REGISTER_NAME_S4:
            s_name = "s4";
            break;
        case REGISTER_NAME_S5:
            s_name = "s5";
            break;
        case REGISTER_NAME_S6:
            s_name = "s6";
            break;
        case REGISTER_NAME_S7:
            s_name = "s7";
            break;
        case REGISTER_NAME_S8:
            s_name = "s8";
            break;
        case REGISTER_NAME_S9:
            s_name = "s9";
            break;
        case REGISTER_NAME_S10:
            s_name = "s10";
            break;
        case REGISTER_NAME_S11:
            s_name = "s11";
            break;
        case REGISTER_NAME_T3:
            s_name = "t3";
            break;
        case REGISTER_NAME_T4:
            s_name = "t4";
            break;
        case REGISTER_NAME_T5:
            s_name = "t5";
            break;
        case REGISTER_NAME_T6:
            s_name = "t6";
            break;
        case REGISTER_NAME_NONE:
            s_name = "none";
            break;
    }
    return s_name;
}

std::string Marlin::oper_name(OperName name)
{
    std::string s_name = " no name ";

    switch (name)
    {
        case OPER_NAME_LUI:
            s_name = "LUI";
            break;
        case OPER_NAME_AUIPC:
            s_name = "AUIPC";
            break;
        case OPER_NAME_JAL:
            s_name = "JAL";
            break;
        case OPER_NAME_JALR:
            s_name = "JALR";
            break;
        case OPER_NAME_BEQ:
            s_name = "BEQ";
            break;
        case OPER_NAME_BNE:
            s_name = "BNE";
            break;
        case OPER_NAME_BLT:
            s_name = "BLT";
            break;
        case OPER_NAME_BGE:
            s_name = "BGE";
            break;
        case OPER_NAME_BLTU:
            s_name = "BLTU";
            break;
        case OPER_NAME_BGEU:
            s_name = "BGEU";
            break;
        case OPER_NAME_LB:
            s_name = "LB";
            break;
        case OPER_NAME_LH:
            s_name = "LH";
            break;
        case OPER_NAME_LW:
            s_name = "LW";
            break;
        case OPER_NAME_LBU:
            s_name = "LBU";
            break;
        case OPER_NAME_LHU:
            s_name = "LHU";
            break;
        case OPER_NAME_SB:
            s_name = "SB";
            break;
        case OPER_NAME_SH:
            s_name = "SH";
            break;
        case OPER_NAME_SW:
            s_name = "SW";
            break;
        case OPER_NAME_ADDI:
            s_name = "ADDI";
            break;
        case OPER_NAME_SLTI:
            s_name = "SLTI";
            break;
        case OPER_NAME_SLTIU:
            s_name = "SLTIU";
            break;
        case OPER_NAME_XORI:
            s_name = "XORI";
            break;
        case OPER_NAME_ORI:
            s_name = "ORI";
            break;
        case OPER_NAME_ANDI:
            s_name = "ANDI";
            break;
        case OPER_NAME_SLLI:
            s_name = "SLLI";
            break;
        case OPER_NAME_SRLI:
            s_name = "SRLI";
            break;
        case OPER_NAME_SRAI:
            s_name = "SRAI";
            break;
        case OPER_NAME_ADD:
            s_name = "ADD";
            break;
        case OPER_NAME_SUB:
            s_name = "SUB";
            break;
        case OPER_NAME_SLL:
            s_name = "SLL";
            break;
        case OPER_NAME_SLT:
            s_name = "SLT";
            break;
        case OPER_NAME_SLTU:
            s_name = "SLTU";
            break;
        case OPER_NAME_XOR:
            s_name = "XOR";
            break;
        case OPER_NAME_SRL:
            s_name = "SRL";
            break;
        case OPER_NAME_SRA:
            s_name = "SRA";
            break;
        case OPER_NAME_OR:
            s_name = "OR";
            break;
        case OPER_NAME_AND:
            s_name = "AND";
            break;
        case OPER_NAME_FENCE:
            s_name = "FENCE";
            break;
        case OPER_NAME_FENCE_I:
            s_name = "FENCE_I";
            break;
        case OPER_NAME_ECALL:
            s_name = "ECALL";
            break;
        case OPER_NAME_EBREAK:
            s_name = "EBREAK";
            break;
        case OPER_NAME_CSRRW:
            s_name = "CSRRW";
            break;
        case OPER_NAME_CSRRS:
            s_name = "CSRRS";
            break;
        case OPER_NAME_CSRRC:
            s_name = "CSRRC";
            break;
        case OPER_NAME_CSRRWI:
            s_name = "CSRRWI";
            break;
        case OPER_NAME_CSRRSI:
            s_name = "CSRRSI";
            break;
        case OPER_NAME_CSRRCI:
            s_name = "CSRRCI";
            break;
        case OPER_NAME_NONE:
            s_name = "NONE";
            break;
    }
    return s_name;
}