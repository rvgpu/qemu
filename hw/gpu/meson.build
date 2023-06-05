rvgpu_ss = ss.source_set()
rvgpu_ss.add(files(
    'dma.c',
    'irq.c',
    'mmio.c',
    'pciemu.c',
))

softmmu_ss.add_all(when: 'CONFIG_RVGPU', if_true: rvgpu_ss)
