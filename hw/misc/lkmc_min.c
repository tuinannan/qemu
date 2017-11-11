/*
TODO this can be inserted in QEMU with -device, but I haven't managed to get
a kernel module to interact with it.
*/

#include "qemu/osdep.h"
#include "hw/qdev.h" /* DeviceClass, DeviceState */

typedef struct {
    DeviceState parent_obj;
    MemoryRegion mmio;
} State;

static uint64_t mmio_read(void *opaque, hwaddr addr, unsigned size)
{
    printf("mmio_read addr = %llx size = %x", (unsigned long long)addr, size);
    return 0x1234567812345678;
}

static void mmio_write(void *opaque, hwaddr addr, uint64_t val,
                unsigned size)
{
    /*State *state = opaque;*/

    printf("mmio_read addr = %llx val = %llx size = %x\n",
            (unsigned long long)addr, (unsigned long long)val, size);
    switch (addr) {
        case 0x0:
            /*pci_set_irq(&state->pdev, 1);*/
        break;
        case 0x4:
            /*pci_set_irq(&state->pdev, 0);*/
        break;
    }
}

static const MemoryRegionOps mmio_ops = {
    .read = mmio_read,
    .write = mmio_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void realize(DeviceState *device_state, Error **errp)
{
    State *state = DO_UPCAST(State, parent_obj, device_state);

    /*pci_config_set_interrupt_pin(pdev->config, 1);*/
    memory_region_init_io(&state->mmio, OBJECT(state), &mmio_ops, state, "lkmc_min_mmio", 8);
    /*printf("mmio = %llx\n", &state->mmio)*/
    /*pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &state->mmio);*/
}

static void class_init(ObjectClass *class, void *data)
{
    DeviceClass *k = DEVICE_CLASS(class);

    k->realize = realize;
}

static const TypeInfo type_info = {
    .name          = "lkmc_min",
    .parent        = TYPE_DEVICE,
    .instance_size = sizeof(State),
    .class_init    = class_init,
};

static void register_types(void)
{
    type_register_static(&type_info);
}

type_init(register_types)
