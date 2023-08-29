/**
 * Export C interface of rvgpu, need link to librvgpu.so
 */

#ifndef RVGPU_H__
#define RVGPU_H__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void *rvgpu_create(void);
void rvgpu_destroy(void *gpu);


void rvgpu_write_vram(void *gpu, uint64_t addr, uint64_t data, uint32_t size);
uint64_t rvgpu_read_vram(void *gpu, uint64_t addr, uint32_t size);
void rvgpu_write_register(void *gpu, uint64_t addr, uint64_t data, uint32_t size);
uint64_t rvgpu_read_register(void *gpu, uint64_t addr, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif // RVGPU_H__
