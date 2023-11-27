rvgpu_ss = ss.source_set()
rvgpu_ss.add(files(
    'rvgpu_dev.c',
    'rvgpu_cif.cpp'
))

librvgpu = dependency('librvgsim', version: '>=1.0', required: true, method: 'pkg-config', kwargs: static_kwargs)
rvgpu_ss.add(librvgpu)

softmmu_ss.add_all(when: 'CONFIG_RVGPU', if_true: rvgpu_ss)
