#include "imxrt.h"
#include <stdint.h>

struct Region {
    // Access attributes are split up into two access types unless
    // privilged and unprivileged access levels are the same.
    //
    // PrivilegedAccess+UnprivilegedAccess
    enum class Attributes : uint32_t {
        /* Access Attributes */
        NoAccess = SCB_MPU_RASR_AP(0),
        PrivilegedAccessOnly = SCB_MPU_RASR_AP(1),
        ReadWriteReadOnly = SCB_MPU_RASR_AP(2),
        ReadWrite = SCB_MPU_RASR_AP(3),
        ReadOnlyNoAccess = SCB_MPU_RASR_AP(5),
        ReadOnly = SCB_MPU_RASR_AP(7),
        NoExecute = SCB_MPU_RASR_XN,
        /* Cache Attributes */
        MemoryCacheWriteThrough = SCB_MPU_RASR_TEX(0) | SCB_MPU_RASR_C,
        MemoryCacheWriteBack = SCB_MPU_RASR_TEX(0) | SCB_MPU_RASR_C | SCB_MPU_RASR_B,
        MemoryCacheWriteBackAllocate = SCB_MPU_RASR_TEX(1) | SCB_MPU_RASR_C | SCB_MPU_RASR_B,
        MemoryNoCache = SCB_MPU_RASR_TEX(1),
        DeviceNoCache = SCB_MPU_RASR_TEX(2)
    };

    enum class Size : uint32_t {
        B32 = SCB_MPU_RASR_SIZE(4),
        B64 = SCB_MPU_RASR_SIZE(5),
        B128 = SCB_MPU_RASR_SIZE(6),
        B256 = SCB_MPU_RASR_SIZE(7),
        B512 = SCB_MPU_RASR_SIZE(8),
        K1 = SCB_MPU_RASR_SIZE(9),
        K2 = SCB_MPU_RASR_SIZE(10),
        K4 = SCB_MPU_RASR_SIZE(11),
        K8 = SCB_MPU_RASR_SIZE(12),
        K16 = SCB_MPU_RASR_SIZE(13),
        K32 = SCB_MPU_RASR_SIZE(14),
        K64 = SCB_MPU_RASR_SIZE(15),
        K128 = SCB_MPU_RASR_SIZE(16),
        K256 = SCB_MPU_RASR_SIZE(17),
        K512 = SCB_MPU_RASR_SIZE(18),
        M1 = SCB_MPU_RASR_SIZE(19),
        M2 = SCB_MPU_RASR_SIZE(20),
        M4 = SCB_MPU_RASR_SIZE(21),
        M8 = SCB_MPU_RASR_SIZE(22),
        M16 = SCB_MPU_RASR_SIZE(23),
        M32 = SCB_MPU_RASR_SIZE(24),
        M64 = SCB_MPU_RASR_SIZE(25),
        M128 = SCB_MPU_RASR_SIZE(26),
        M256 = SCB_MPU_RASR_SIZE(27),
        M512 = SCB_MPU_RASR_SIZE(28),
        G1 = SCB_MPU_RASR_SIZE(29),
        G2 = SCB_MPU_RASR_SIZE(30),
        G4 = SCB_MPU_RASR_SIZE(31)
    };

    uint8_t regionNumber;
    uint32_t regionStartAddress;
    Size regionSize;
    Attributes attributes;
    bool valid;
};

/*
 * A side note:
 *  - The current number of regions taken
 *  - up by the Teensy 4.1 core is 10
 *  - (starting from Region 0)
 */

class MPU {
    public:
        // Control methods
        static void Enable();

        static void Disable();

        static void SetPrivilegedDefault(bool privdefena);

        static void SetHardFaultNMI(bool hfnmiena);

        static int ConfigureRegion(const Region &region);

        // Status methods
        static bool GetEnabled();

        static Region GetRegionConfiguration(uint8_t regionNumber);

        static uint8_t GetInstructionRegion();

        static uint8_t GetSupportedRegions();
        
        static bool GetSeperateSupport();
};

inline Region::Attributes operator|(Region::Attributes first, Region::Attributes second) {
    return static_cast<Region::Attributes>(static_cast<uint32_t>(first) | static_cast<uint32_t>(second));
}

inline Region::Attributes operator&(Region::Attributes first, Region::Attributes second) {
    return static_cast<Region::Attributes>(static_cast<uint32_t>(first) & static_cast<uint32_t>(second));
}
