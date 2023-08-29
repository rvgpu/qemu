#include <cstdio>

#include "rvgpu.h"
#include "rvgpu_cif.h"

void *rvgpu_create(void)
{
    return (void *)new rvgpu();
}

void rvgpu_destroy(void *gpu)
{
    delete static_cast<rvgpu *>(gpu);
}

void rvgpu_write_vram(void *gpu, uint64_t addr, uint64_t data, uint32_t size)
{
    rvgpu *rgpu = static_cast<rvgpu *>(gpu);
    rgpu->write_vram(addr, data, size);
}

uint64_t rvgpu_read_vram(void *gpu, uint64_t addr, uint32_t size)
{
    rvgpu *rgpu = static_cast<rvgpu *>(gpu);
    uint64_t rdata = 0;
    rdata = (uint64_t)rgpu->read_vram(addr, size);

    return rdata;
}

void rvgpu_write_register(void *gpu, uint64_t addr, uint64_t data, uint32_t size)
{
    rvgpu *rgpu = static_cast<rvgpu *>(gpu);

    if (size != 4) {
        printf("register need r/w with size 4\n");
        return;
    }

    rgpu->write_register(addr, uint32_t(data));
}

uint64_t rvgpu_read_register(void *gpu, uint64_t addr, uint32_t size)
{
    rvgpu *rgpu = static_cast<rvgpu *>(gpu);
    uint64_t rdata = 0;

    if (size != 4) {
        printf("register need r/w with size 4\n");
        return rdata;
    }

    rdata = (uint64_t)rgpu->read_register(addr);

    return rdata;
}
