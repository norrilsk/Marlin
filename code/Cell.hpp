#ifndef MARLIN_CELL_HPP
#define MARLIN_CELL_HPP
#include <stdexcept>

//predecl
class Oper;
struct WF // special cell for Fetch take data from
{
    bool is_stall = false;
    bool is_hazard_stall = false;
    bool is_jump = false;
    uint32_t pc = 0;
    uint32_t pc_jump=0;
    void clear()
    {
        is_stall =false;
        is_hazard_stall = false;
        is_jump = false;
        pc = 0;
        pc_jump = 0;
    }
};
struct FD //Fetch-Decode
{
    bool is_stall = true;
    uint32_t pc = 0;
    uint32_t instr = 0;
    bool is_hazard_stall = false;
    void clear()
    {
        is_stall =true;
        is_hazard_stall = false;
        pc = 0;
        instr = 0;
    }
    
};
struct DE //Decode -Execute
{
    bool is_stall = true;
    bool is_hazard_stall = false;
    uint32_t pc = 0;
    Oper * op = nullptr;
    void clear()
    {
        is_stall =true;
        is_hazard_stall = false;
        pc = 0;
        op = nullptr;
    }
};
struct EM //Execute - memory access
{
    bool is_stall = true;
    bool is_hazard_stall = false;
    uint32_t pc = 0;
    Oper * op = nullptr;
    void clear()
    {
        is_stall =true;
        is_hazard_stall = false;
        pc = 0;
        op = nullptr;
    }
};

struct MW
{
    bool is_stall = true;
    bool is_hazard_stall = false;
    uint32_t pc =0;
    Oper* op = nullptr;
    void clear()
    {
        is_stall =true;
        is_hazard_stall = false;
        pc = 0;
        op = nullptr;
    }
};

template <typename T>
class Cell
{
private:
    T* phase1;
    T* phase2;
    friend class HazartUnit;
public:
    void update();
    T* get_store_ptr(){return phase1;}
    T* get_load_ptr(){return phase2;}
    void store(T* t);
    void load(T* t);
    Cell();
    ~Cell();
};

template<typename T>
Cell<T>::Cell()
{
    phase1 = new T;
    phase2 = new T;
    if (!phase1 || !phase2)
        throw std::runtime_error("allocation error");
}
template<typename T>
Cell<T>::~Cell()
{
    delete phase1;
    delete phase2;
}
template<typename T>
void Cell<T>::load(T* t)
{
    *t = *phase2;
}
template<typename T>
void Cell<T>::store(T* t )
{
    *phase1 = *t;
}
template<typename T>
void Cell<T>::update()
{
    T* temp;
    temp = phase1;
    phase1 = phase2;
    phase1->clear();
    phase2 = temp;
}

#endif //MARLIN_CELL_HPP
