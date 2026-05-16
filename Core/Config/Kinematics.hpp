#pragma once
#include <array>
namespace config::motion
{
    struct PersistedData{
        uint16_t index = 0;
        uint8_t subindex = 0;
        size_t size = 4;
        uint8_t address = 0;
        uint32_t value = 0;
    };
    std::array<PersistedData, 11> persisted_data = {
        PersistedData{0x6099, 1, 4,0,0}, // Homing Speed
        PersistedData{0x6099, 2, 4,4,0}, // Search for Homing Switch Acceleration
        PersistedData{0x609A, 0, 4,8,0}, // Search for Index Switch Acceleration
        PersistedData{0x607C, 0, 4,12,0}, // Homing Deceleration
        PersistedData{0x60FF, 0, 4,16,0}, // Target Velocity
        PersistedData{0x6083, 0, 4,20,0}, // Profile Acceleration
        PersistedData{0x6084, 0, 4,24,0}, // Profile Deceleration
        PersistedData{0x6085, 0, 4,28,0}, // Profile Velocity
        PersistedData{0x6080, 0, 4,32,0}, // Max Profile Velocity
        PersistedData{0x607B, 1, 4,36,0}, // Min Software defined Limit 
        PersistedData{0x607B, 2, 4,40,0}  // Max Software defined Limit
    };
}
