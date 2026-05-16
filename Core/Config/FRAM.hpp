#ifndef FRAM_HPP
#define FRAM_HPP

#include "stm32g4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

class FRAM {
public:
    struct DeviceID {
        uint8_t manufacturerId;
        uint8_t continuationCode;
        uint8_t productId1;
        uint8_t productId2;
    };

    FRAM(
        SPI_HandleTypeDef* hspi,
        GPIO_TypeDef* csPort,
        uint16_t csPin
    );

    bool begin();

    DeviceID readDeviceId();

    uint8_t readStatus();

    void writeEnable();
    void writeDisable();

    uint8_t read8(uint32_t address);
    void write8(uint32_t address, uint8_t value);

    void writeBuffer(uint32_t address, const uint8_t* data, uint16_t size);
    void readBuffer(uint32_t address, uint8_t* data, uint16_t size);
    bool testWriteRead(uint32_t address, uint8_t value);

private:
    SPI_HandleTypeDef* _hspi;
    GPIO_TypeDef* _csPort;
    uint16_t _csPin;

    static constexpr uint32_t SPI_TIMEOUT_MS = 100;

    static constexpr uint8_t CMD_WREN  = 0x06;
    static constexpr uint8_t CMD_WRDI  = 0x04;
    static constexpr uint8_t CMD_RDSR  = 0x05;
    static constexpr uint8_t CMD_READ  = 0x03;
    static constexpr uint8_t CMD_WRITE = 0x02;
    static constexpr uint8_t CMD_RDID  = 0x9F;

    void select();
    void deselect();

    HAL_StatusTypeDef transmit(uint8_t* data, uint16_t size);
    HAL_StatusTypeDef receive(uint8_t* data, uint16_t size);
    HAL_StatusTypeDef transmitReceive(uint8_t* txData, uint8_t* rxData, uint16_t size);
};

#endif