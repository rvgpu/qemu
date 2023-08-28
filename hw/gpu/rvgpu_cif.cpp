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
