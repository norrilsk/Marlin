
#include "MMU.hpp"

MMU::MMU(Config& config): config(config), memory(config)
{
    this->config.get_page_size();
}
int MMU::upload_segment_to_mem(void* data,uint64_t addres, uint64_t flags,uint64_t data_size, uint64_t data_size_in_mem)
{
    /*
     * Напишите ваш код Здесь
     */

    (void)flags;
    (void)data_size_in_mem;
    memory.write(data,addres,data_size);
    return 1;
}

int MMU::read_from_mem(void *data, uint64_t address, uint64_t data_size)
{
    memory.read(data,address,data_size);
    return 0;
}
int MMU::write_to_mem(void *data, uint64_t address, uint64_t data_size)
{
    memory.write(data,address,data_size);
    return 0;
}