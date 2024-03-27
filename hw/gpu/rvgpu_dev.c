/**
 * RVGPU Virtual Device Interface
 *
 * References: https://github.com/luizinhosuraty/pciemu
 */

#include "qemu/units.h"

#include "rvgpu_cif.h"
#include "rvgpu_dev.h"

static uint64_t rvgpu_pci_doorbell_read(void *opaque, hwaddr addr, unsigned size)
{
    RVGPUDevice *dev = (RVGPUDevice *)opaque;
    return rvgpu_read_doorbell(dev->rvgpu_class, addr, size);
}

static void rvgpu_pci_doorbell_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    RVGPUDevice *dev = (RVGPUDevice *)opaque;
    rvgpu_write_doorbell(dev->rvgpu_class, addr, val, size);
}

static uint64_t rvgpu_pci_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    RVGPUDevice *dev = (RVGPUDevice *)opaque;
    return rvgpu_read_mmio(dev->rvgpu_class, addr, size);
}

static void rvgpu_pci_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    RVGPUDevice *dev = (RVGPUDevice *)opaque;
    rvgpu_write_mmio(dev->rvgpu_class, addr, val, size);
}

static uint64_t rvgpu_pci_io_read(void *opaque, hwaddr addr, unsigned size)
{
    printf("read region4 io: 0x%lx(%d)\n", addr, size);
    return 0;
}

static void rvgpu_pci_io_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    printf("write region4 io: 0x%lx(%d): 0x%lx\n", addr, size, val);
}

static uint64_t rvgpu_pci_vram_read(void *opaque, hwaddr addr, unsigned size)
{
    printf("read region1 vram: 0x%lx(%d)\n", addr, size);
    RVGPUDevice *dev = (RVGPUDevice *)opaque;
    return rvgpu_read_vram(dev->rvgpu_class, addr, size);
}

static void rvgpu_pci_vram_write(void *opaque, hwaddr addr, uint64_t val, unsigned size)
{
    printf("write region1 vram: 0x%lx(%d):%lx\n", addr, size, val);
    RVGPUDevice *dev = (RVGPUDevice *)opaque;
    rvgpu_write_vram(dev->rvgpu_class, addr, val, size);
}

static const MemoryRegionOps rvgpu_vram_ops = {
    .read = rvgpu_pci_vram_read,
    .write = rvgpu_pci_vram_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .impl.min_access_size = 1,
    .impl.max_access_size = 8,
};

static const MemoryRegionOps rvgpu_doorbell_ops = {
    .read = rvgpu_pci_doorbell_read,
    .write = rvgpu_pci_doorbell_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .impl.min_access_size = 1,
    .impl.max_access_size = 4,
};

static const MemoryRegionOps rvgpu_mmio_ops = {
    .read = rvgpu_pci_mmio_read,
    .write = rvgpu_pci_mmio_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .impl.min_access_size = 1,
    .impl.max_access_size = 4,
};

static const MemoryRegionOps rvgpu_io_ops = {
    .read = rvgpu_pci_io_read,
    .write = rvgpu_pci_io_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .impl.min_access_size = 1,
    .impl.max_access_size = 4,
};

/* -----------------------------------------------------------------------------
 *  Internal functions
 * -----------------------------------------------------------------------------
 */

/**
 * rvgpu_reset: resets the RVGPUDevice
 *
 * @dev: Instance of RVGPUDevice object being reset
 */
static void rvgpu_reset(RVGPUDevice *dev)
{
    // rvgpu_irq_reset(dev);
    // rvgpu_dma_reset(dev);
    // rvgpu_mmio_reset(dev);
}

/* -----------------------------------------------------------------------------
 *  Object related functions
 * -----------------------------------------------------------------------------
 */

/**
 * rvgpu_device_init: Device initialization
 *
 * Initializes the newly instantiated RVGPUUDevice object. This can be seen
 * as a constructor of the RVGPUDevice.
 * Note that we receive a pointer for a PCIDevice, but, due to the OOP hack
 * done by the QEMU Object Model, we can easily cast to a RVGPUDevice.
 *
 * @pci_dev: Instance of PCIDevice object being initialized
 * @errp: pointer to indicate errors
 */
static void rvgpu_device_init(PCIDevice *pci_dev, Error **errp)
{
    RVGPUDevice *dev = RVGPU_DEVICE(pci_dev);
    dev->rvgpu_class = rvgpu_create();

    // Bar0: 32G accessed memory
    // Bar2: 2M  accessed memory
    // Bar4: I/O ports, 256B
    // Bar5: 1M  accessed memory
    memory_region_init_io(&dev->vram, OBJECT(dev), &rvgpu_vram_ops, dev, "rvgpu.vram", 32 * 1024 * MiB);
    memory_region_init_io(&dev->doorbell, OBJECT(dev), &rvgpu_doorbell_ops, dev, "rvgpu.doorbell", 2 * MiB);
    memory_region_init_io(&dev->mmio, OBJECT(dev), &rvgpu_mmio_ops, dev, "rvgpu.mmio", 1 * MiB);
    memory_region_init_io(&dev->io,   OBJECT(dev), &rvgpu_io_ops,   dev, "rvgpu.io",   256);
    pci_register_bar(&dev->pci_dev, 0, PCI_BASE_ADDRESS_MEM_PREFETCH | PCI_BASE_ADDRESS_MEM_TYPE_64, &dev->vram);
    pci_register_bar(&dev->pci_dev, 2, PCI_BASE_ADDRESS_MEM_PREFETCH | PCI_BASE_ADDRESS_MEM_TYPE_64, &dev->doorbell);
    pci_register_bar(&dev->pci_dev, 5, PCI_BASE_ADDRESS_MEM_TYPE_32, &dev->mmio);
    pci_register_bar(&dev->pci_dev, 4, PCI_BASE_ADDRESS_SPACE_IO, &dev->io);

    // rvgpu_irq_init(dev, errp);
    // rvgpu_dma_init(dev, errp);
    // rvgpu_mmio_init(dev, errp);
}

/**
 * rvgpu_device_fini: Device finalization
 *
 * Finalizes the instantiated RVGPUDevice object. This can be seen
 * as a destructor of the RVGPUDevice.
 * Note that we receive a pointer for a PCIDevice, but, due to the OOP hack
 * done by the QEMU Object Model, we can easily cast to a RVGPUDevice.
 *
 * @pci_dev: Instance of PCIDevice object being initialized
 */
static void rvgpu_device_fini(PCIDevice *pci_dev)
{
    RVGPUDevice *dev = RVGPU_DEVICE(pci_dev);
    
    rvgpu_destroy(dev->rvgpu_class);
    // rvgpu_irq_fini(dev);
    // rvgpu_dma_fini(dev);
    // rvgpu_mmio_fini(dev);
}

/**
 * rvgpu_device_reset: Device reset
 *
 * Note that we receive a pointer for a DeviceState, but, due to the OOP hack
 * done by the QEMU Object Model, we can easily cast to a RVGPUDevice.
 *
 * @dev: Instance of DeviceState object being reset
 */
static void rvgpu_device_reset(DeviceState *dev)
{
    rvgpu_reset(RVGPU_DEVICE(dev));
}


/* -----------------------------------------------------------------------------
 *  Class related functions
 * -----------------------------------------------------------------------------
 */

/**
 * rvgpu_class_init: Class initialization
 *
 * Initializes the class RVGPUDeviceClass.
 * Note that we receive a pointer for an ObjectClass, but, due to the OOP hack
 * done by the QEMU Object Model, we can easily cast to a RVGPUDeviceClass.
 *
 * @klass: ObjectClass being initialized
 * @class_data: the data passed during initialization
 */
static void rvgpu_class_init(ObjectClass *klass, void *class_data)
{
    DeviceClass *device_class = DEVICE_CLASS(klass);
    PCIDeviceClass *pci_device_class = PCI_DEVICE_CLASS(klass);

    pci_device_class->realize = rvgpu_device_init;
    pci_device_class->exit = rvgpu_device_fini;
    pci_device_class->vendor_id = RVGPU_HW_VENDOR_ID;
    pci_device_class->device_id = RVGPU_HW_DEVICE_ID;
    pci_device_class->revision = RVGPU_HW_REVISION;
    pci_device_class->class_id = PCI_CLASS_DISPLAY_3D;

    set_bit(DEVICE_CATEGORY_DISPLAY, device_class->categories);
    device_class->desc = RVGPU_DEVICE_DESC;
    device_class->reset = rvgpu_device_reset;
}

/* -----------------------------------------------------------------------------
 *  Declaration, definition and registration of type information
 * -----------------------------------------------------------------------------
 */

/**
 * rvgpu_info: Description of the new rvgpu type
 *
 * Specifies the rvgpu type with information about RVGPUDevice object and
 * how to initialize the class (RVGPUDeviceClass), the parent of this object
 * and the interfaces it eventually implements.
 * This is information is used to create the ObjectClass and Object instance
 * of the RVGPU device.
 *
 * Check [1] for more information on the QEMU Object Model (QOM).
 *
 * References:
 *  [1] https://qemu.readthedocs.io/en/latest/devel/qom.html
 *
 */
static const TypeInfo rvgpu_info = {
    .name = TYPE_RVGPU_DEVICE,
    .parent = TYPE_PCI_DEVICE,
    .instance_size = sizeof(RVGPUDevice),
    .class_init = rvgpu_class_init,
    .interfaces =
        (InterfaceInfo[]){
            { INTERFACE_PCIE_DEVICE },
            {},
        },
};


/**
 * rvgpu_register_types: Register the rvgpu type with QOM
 *
 * With the registration of the rvgpu type, QEMU can now create the
 * ObjectClass (RVGPUDeviceClass) and Object (RVGPUDevice) instance
 * with the correct information on their size, the parent relationship
 * with other instances and how to initialize them.
 *
 * Check [1] for more information.
 *
 * References:
 *  [1] https://qemu.readthedocs.io/en/latest/devel/qom.html
 *
 */
static void rvgpu_register_types(void)
{
    type_register_static(&rvgpu_info);
}

type_init(rvgpu_register_types)
