/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HW_TESTDEVICE_H
#define HW_TESTDEVICE_H

#include "hw/core/sysbus.h"
#include "qom/object.h"

#define TYPE_TESTDEVICE "testdevice"
OBJECT_DECLARE_SIMPLE_TYPE(TestMiscState, TESTDEVICE)


struct TestMiscState {
    SysBusDevice parent_obj;

    MemoryRegion iomem;
    uint32_t reg0;
    uint32_t reg1;
};

#endif
