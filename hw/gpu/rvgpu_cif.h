/**
 * Export C interface of rvgpu, need link to librvgpu.so
 */

#ifndef RVGPU_H__
#define RVGPU_H__

#ifdef __cplusplus
extern "C" {
#endif

void *rvgpu_create(void);
void rvgpu_destroy(void *gpu);

#ifdef __cplusplus
}
#endif

#endif // RVGPU_H__
