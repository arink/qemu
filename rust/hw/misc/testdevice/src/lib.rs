// Copyright (C) 2024 The QEMU Project Developers.
// SPDX-License-Identifier: GPL-2.0-or-later

//! # Test Misc QEMU Device Model
//!
//! A minimal example device with two 32-bit memory-mapped registers
//! located at offset `0x00` and `0x04`. Both registers are readable
//! and writable, and serve as a starting point for writing QEMU
//! devices in Rust.

mod bindings;
mod device;


pub const TYPE_TESTDEVICE: &::std::ffi::CStr = c"testdevice";

