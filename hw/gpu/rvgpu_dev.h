/**
 * RVGPU Virtual Device Interface
 *
 * References: https://github.com/luizinhosuraty/pciemu
 */

#ifndef RVGPU_DEV_H
#define RVGPU_DEV_H

#include "qemu/osdep.h"
#include "hw/pci/pci.h"
#include "hw/pci/pci_device.h"

#define TYPE_RVGPU_DEVICE "rvgpu"
#define RVGPU_DEVICE_DESC "RVGPU PCIe Device"

#define RVGPU_HW_VENDOR_ID 0x1b36
#define RVGPU_HW_DEVICE_ID 0x1100
#define RVGPU_HW_REVISION 0x01

/*
 * Declare the object type for RVGPUDevice and all boilerplate code
 * See https://qemu.readthedocs.io/en/latest/devel/qom.html for details
 *
 */
OBJECT_DECLARE_TYPE(RVGPUDevice, RVGPUDeviceClass, RVGPU_DEVICE);

/* Struct that defines our class
 */
typedef struct RVGPUDeviceClass {
    /*
     * OOP hack : Our parent class (PCIDeviceClass) is part of the strutct
     * The idea is to be able to access it from our own class
     */
    PCIDeviceClass parent_class;
} RVGPUDeviceClass;

typedef struct RVGPUDevice {
    /*< private >*/
    /* OOP hack : Our parent (PCIDevice) is part of the struct */
    PCIDevice pci_dev;
    /*< public >*/

    // Memory Region
    MemoryRegion vram;
    MemoryRegion reg;


    void *rvgpu_class;
} RVGPUDevice;

#endif /* RVGPU_DEV_H */
