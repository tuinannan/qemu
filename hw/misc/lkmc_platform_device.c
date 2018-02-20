/*
Device interface:

Register range: 0x0 - 0x1000

Read: return 0x12340000 + requested offset

Write: toggle IRQ on / off
*/

#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "qemu/log.h"

#define TYPE_LKMC_PLATFORM_DEVICE "lkmc_platform_device"
#define LKMC_PLATFORM_DEVICE(obj) OBJECT_CHECK(LkmcPlatformDeviceState, (obj), TYPE_LKMC_PLATFORM_DEVICE)

typedef struct LkmcPlatformDeviceState {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    qemu_irq irq;
    int irq_level;
} LkmcPlatformDeviceState;

static uint64_t lkmc_platform_device_read(void *opaque, hwaddr offset,
                           unsigned size)
{
    printf("lkmc_platform_device_read offset=%llx size=%llx\n",
        (unsigned long long)offset, (unsigned long long)size);
    return 0x12340000 + offset;
}

static void lkmc_platform_device_write(void *opaque, hwaddr offset,
                        uint64_t value, unsigned size)
{
    LkmcPlatformDeviceState *s = (LkmcPlatformDeviceState *)opaque;

    printf("lkmc_platform_device_write offset=%llx value=%llx size=%llx\n",
        (unsigned long long)offset, (unsigned long long)value, (unsigned long long)size);
    s->irq_level = !s->irq_level;
    qemu_set_irq(s->irq, s->irq_level);
}

static const MemoryRegionOps lkmc_platform_device_ops = {
    .read = lkmc_platform_device_read,
    .write = lkmc_platform_device_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void lkmc_platform_device_init(Object *obj)
{
    LkmcPlatformDeviceState *s = LKMC_PLATFORM_DEVICE(obj);
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);

    s->irq_level = 0;
    memory_region_init_io(&s->iomem, obj, &lkmc_platform_device_ops, s, "lkmc_platform_device", 0x1000);
    sysbus_init_mmio(sbd, &s->iomem);
    sysbus_init_irq(sbd, &s->irq);
}

static const TypeInfo lkmc_platform_device_info = {
    .name          = TYPE_LKMC_PLATFORM_DEVICE,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(LkmcPlatformDeviceState),
    .instance_init = lkmc_platform_device_init,
};

static void lkmc_platform_device_register_types(void)
{
    type_register_static(&lkmc_platform_device_info);
}

type_init(lkmc_platform_device_register_types)
