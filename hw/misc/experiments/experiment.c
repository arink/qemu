#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/units.h"
#include "hw/pci/pci.h"
#include "hw/pci/msi.h"

#define TYPE_PCI_EXPERIMENT_DEVICE "experiment"
typedef struct ExperimentState ExperimentState;
DECLARE_INSTANCE_CHECKER(ExperimentState, EXPERIMENT,
                         TYPE_PCI_EXPERIMENT_DEVICE)

struct ExperimentState {
    PCIDevice pdev;
    MemoryRegion mmio;
};

static uint64_t experiment_mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    //ExperimentState *experiment = opaque;
    uint64_t val = ~0ULL;

    switch (addr) {
        case 0:
            val = 0xface;
            break;

        case 4:
            val = 0x9876;
            break;

        default:
            val = addr;
            break;
    }

    qemu_log_mask(LOG_GUEST_ERROR,
                  "experiment: mmio read @0x%" HWADDR_PRIx " size=%u. Result 0x%lx\n",
                  addr, size, val);

    // TODO

    return val;
}

static void experiment_mmio_write(void *opaque, hwaddr addr, uint64_t val,
                unsigned size)
{
    //ExperimentState *experiment = opaque;

    qemu_log_mask(LOG_GUEST_ERROR,
                  "experiment: mmio write @0x%" HWADDR_PRIx " val=0x%" PRIx64
                  " size=%u\n",
                  addr, val, size);

    // TODO
}

static const MemoryRegionOps experiment_mmio_ops = {
    .read = experiment_mmio_read,
    .write = experiment_mmio_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .valid = {
        .min_access_size = 4,
        .max_access_size = 8,
    },
    .impl = {
        .min_access_size = 4,
        .max_access_size = 8,
    },

};

static void pci_experiment_realize(PCIDevice *pdev, Error **errp)
{
    ExperimentState *experiment = EXPERIMENT(pdev);
    uint8_t *pci_conf = pdev->config;

    pci_config_set_interrupt_pin(pci_conf, 1);

    if (msi_init(pdev, 0, 1, true, false, errp)) {
        return;
    }


    memory_region_init_io(&experiment->mmio, OBJECT(experiment), &experiment_mmio_ops, 
                    experiment,
                    "experiment-mmio", 1 * MiB);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &experiment->mmio);
}

static void pci_experiment_uninit(PCIDevice *pdev)
{
    //ExperimentState *experiment = EXPERIMENT(pdev);
    msi_uninit(pdev);
}

static void experiment_instance_init(Object *obj)
{
    //ExperimentState *experiment = EXPERIMENT(obj);
}

static void experiment_class_init(ObjectClass *class, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(class);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

    k->realize = pci_experiment_realize;
    k->exit = pci_experiment_uninit;
    k->vendor_id = 0x1234;
    k->device_id = 0xCAFE;
    k->revision = 0x10;
    k->class_id = PCI_CLASS_OTHERS;
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo experiment_types[] = {
    {
        .name          = TYPE_PCI_EXPERIMENT_DEVICE,
        .parent        = TYPE_PCI_DEVICE,
        .instance_size = sizeof(ExperimentState),
        .instance_init = experiment_instance_init,
        .class_init    = experiment_class_init,
        .interfaces    = (const InterfaceInfo[]) {
            { INTERFACE_CONVENTIONAL_PCI_DEVICE },
            { },
        },
    }
};

DEFINE_TYPES(experiment_types)
