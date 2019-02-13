
#include "Marlin.hpp"
#include "Config.hpp"
#include<string>

int main()
{
//    Config config;
    Marlin marlin(std::string("/localdisk/Marlin/tests/binary_RV32/base_arithmetic_test")\
    , std::string("../../conf.txt"));
}