#ifndef MARLIN_CELL_HPP
#define MARLIN_CELL_HPP
#include <stdexcept>

template <typename T>
class Cell
{
private:
    T* phase1;
    T* phase2;
public:
    void update();
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
    phase2 = temp;
}

#endif //MARLIN_CELL_HPP
