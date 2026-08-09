// Copyright (C) 2024 The QEMU Project Developers.
// SPDX-License-Identifier: GPL-2.0-or-later

use bql::prelude::*;
use hwcore::prelude::*;
use qom::prelude::*;
use system::prelude::*;
use common::prelude::*;
use util::prelude::*;

/// Size of the device's MMIO register space (two 32-bit registers).
const TESTDEVICE_MMIO_SIZE: u64 = 0x8;

/// Offset of register 0.
const REG0_OFFSET: u32 = 0x0;
/// Offset of register 1.
const REG1_OFFSET: u32 = 0x4;

/// A minimal device exposing two 32-bit registers.
#[repr(C)]
#[derive(qom::Object, hwcore::Device)]
pub struct TestMiscState {
    pub parent_obj: ParentField<SysBusDevice>,
    pub iomem: MemoryRegion,

    /// Register at offset `0x00`.
    reg0: BqlCell<u32>,
    /// Register at offset `0x04`.
    reg1: BqlCell<u32>,
}

impl TestMiscState {
    fn read(&self, addr: hwaddr, _size: u32) -> u64 {
        log_mask_ln!(Log::GuestError, "TestDevice::read: offset");
        match u32::try_from(addr).unwrap() {
            REG0_OFFSET => self.reg0.get() as u64,
            REG1_OFFSET => self.reg1.get() as u64,
            _ => {
                eprintln!("test_misc: read from invalid offset 0x{0:x}", addr);
                0
            }
        }
    }

    fn write(&self, addr: hwaddr, value: u64, _size: u32) {
        let value = value as u32;
        log_mask_ln!(Log::GuestError, "TestDevice::write: offset 0x{0:x} with value 0x{1:x}", addr, value);
        match u32::try_from(addr).unwrap() {
            REG0_OFFSET => self.reg0.set(value),
            REG1_OFFSET => self.reg1.set(value),
            _ => eprintln!("test_misc: write to invalid offset 0x{addr:x}"),
        }
    }

    unsafe fn init(mut this: ParentInit<Self>) {
        static TESTDEVICE_RAM_OPS: MemoryRegionOps<TestMiscState> =
            MemoryRegionOpsBuilder::<TestMiscState>::new()
                .read(&TestMiscState::read)
                .write(&TestMiscState::write)
                .little_endian()
                .valid_sizes(4, 4)
                .impl_sizes(4, 4)
                .build();

        MemoryRegion::init_io(
            &mut uninit_field_mut!(*this, iomem),
            &TESTDEVICE_RAM_OPS,
            "testdevice",
            TESTDEVICE_MMIO_SIZE,
        );
    }
}

qom_isa!(TestMiscState: SysBusDevice, DeviceState, Object);

unsafe impl ObjectType for TestMiscState {
    type Class = <SysBusDevice as ObjectType>::Class;
    const TYPE_NAME: &'static ::std::ffi::CStr = crate::TYPE_TESTDEVICE;
}

impl ObjectImpl for TestMiscState {
    type ParentType = SysBusDevice;

    const INSTANCE_INIT: Option<unsafe fn(ParentInit<Self>)> = Some(Self::init);
    const CLASS_INIT: fn(&mut Self::Class) = Self::Class::class_init::<Self>;
}

impl DeviceImpl for TestMiscState {
    const REALIZE: Option<fn(&Self) -> util::Result<()>> = Some(|_| Ok(()));
}

impl ResettablePhasesImpl for TestMiscState {}

impl SysBusDeviceImpl for TestMiscState {}


