#pragma once

#include "Kinematics.hpp"
#include "FRAM.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

extern "C"
{
    #include "spi.h"
    #include "gpio.h"
}

/*
Embedded backend for reading/writing object dictionary to I2C/SPI FRAM.
*/
namespace config::backends
{
    class FRAMBackend : public FRAM
    {
    public:
        struct TransmissionParameters
        {
            uint8_t nBytes = 0;
            uint8_t* data = nullptr;
            uint32_t address = 0;
        };

        explicit FRAMBackend(
            SPI_HandleTypeDef* hspi,
            GPIO_TypeDef* csPort,
            uint16_t csPin
        )
            : FRAM(hspi, csPort, csPin)
        {
            begin();
        }

        template<typename T>
        auto Read(const uint32_t address) -> T
        {
            static_assert(std::is_trivially_copyable_v<T>,
                          "Read<T> requires trivially copyable type");

            std::array<uint8_t, sizeof(T)> rxBuffer{};

            readBuffer(address, rxBuffer.data(), rxBuffer.size());

            T value{};
            std::memcpy(&value, rxBuffer.data(), sizeof(T));

            return value;
        }

        template<typename T>
        auto Write(const uint32_t address, const T& value) -> bool
        {
            static_assert(std::is_trivially_copyable_v<T>,
                          "Write<T> requires trivially copyable type");

            writeBuffer(
                address,
                reinterpret_cast<const uint8_t*>(&value),
                sizeof(T)
            );

            return true;
        }
    };
}