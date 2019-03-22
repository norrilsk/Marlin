
#include "Marlin.hpp"
#include "Config.hpp"
#include<string>

int main()
{
    Marlin marlin(std::string("../../tests/binary_RV32/empty_test")\
    , std::string("../../conf.txt"));
    marlin.run();
}