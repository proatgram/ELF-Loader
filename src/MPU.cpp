#include "MPU.hpp"

void MPU::Enable() {
    // allow a few cycles for bus writes before enable MPU
	asm("nop"); 
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

    SCB_MPU_CTRL |= 1;
}

void MPU::Disable() {
    SCB_MPU_CTRL &= ~1;
}

void MPU::SetPrivilegedDefault(bool privdefena) {
    SCB_MPU_CTRL &= ~(static_cast<uint32_t>(privdefena) << 1);
}

void MPU::SetHardFaultNMI(bool hfnmiena) {
    SCB_MPU_CTRL &= ~(static_cast<uint32_t>(hfnmiena) << 2);
}

int MPU::ConfigureRegion(const Region &region) {
    if (GetEnabled()) {
        return 1;
    }

    if (!region.valid) {
        SCB_MPU_RNR = region.regionNumber;
    }

    SCB_MPU_RBAR = region.regionStartAddress | SCB_MPU_RBAR_REGION(region.regionNumber) | (static_cast<uint32_t>(region.valid) << 4);
    SCB_MPU_RASR = static_cast<uint32_t>(region.attributes) | static_cast<uint32_t>(region.regionSize) | SCB_MPU_RASR_ENABLE;

    return 0;
}


bool MPU::GetEnabled() {
    return SCB_MPU_CTRL & 1;
}

Region MPU::GetRegionConfiguration(uint8_t regionNumber) {
    Region region;
    region.regionNumber = regionNumber;

    SCB_MPU_RNR = regionNumber;

    region.attributes = static_cast<Region::Attributes>(SCB_MPU_RASR & 0b11111111111111110000000000000000);
    region.regionSize = static_cast<Region::Size>(SCB_MPU_RASR & 0b00000000000000000000000000111110);
    region.regionStartAddress = SCB_MPU_RBAR & SCB_MPU_RBAR_ADDR_MASK;

    return region;
}

uint8_t MPU::GetInstructionRegion() {
    return static_cast<uint8_t>((SCB_MPU_TYPE & 0b00000000111111110000000000000000) >> 16);
}

uint8_t MPU::GetSupportedRegions() {
    return static_cast<uint8_t>((SCB_MPU_TYPE & 0b00000000000000001111111100000000) >> 8);
}

bool MPU::GetSeperateSupport() {
    return static_cast<bool>(SCB_MPU_CTRL & 1);
}
