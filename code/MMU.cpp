
#include "MMU.hpp"

MMU::MMU(Config config): config(config)
{
    memory = Memory(config);
}
int MMU::upload_segment_to_mem(void* data,uint64_t addres, uint64_t flags,uint64_t data_size, uint64_t data_size_in_mem)
{
    /*
     * Напишите ваш код Здесь
     */
    (void)flags;
    (void)data_size_in_mem;
    memory.upload(data,addres,data_size);
    return 1;
}