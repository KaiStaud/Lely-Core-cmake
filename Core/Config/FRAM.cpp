#include "FRAM.hpp"
#include <cstring>
FRAM::FRAM(
    SPI_HandleTypeDef* hspi,
    GPIO_TypeDef* csPort,
    uint16_t csPin
) :
    _hspi(hspi),
    _csPort(csPort),
    _csPin(csPin)
{
}

bool FRAM::begin() {
    deselect();

    DeviceID id = readDeviceId();

    if (id.manufacturerId == 0x04 &&
        id.continuationCode == 0x7F) {
        return true;
    }

    return false;
}

void FRAM::select() {
    HAL_GPIO_WritePin(_csPort, _csPin, GPIO_PIN_RESET);
}

void FRAM::deselect() {
    HAL_GPIO_WritePin(_csPort, _csPin, GPIO_PIN_SET);
}

HAL_StatusTypeDef FRAM::transmit(uint8_t* data, uint16_t size) {
    return HAL_SPI_Transmit(_hspi, data, size, SPI_TIMEOUT_MS);
}

HAL_StatusTypeDef FRAM::receive(uint8_t* data, uint16_t size) {
    return HAL_SPI_Receive(_hspi, data, size, SPI_TIMEOUT_MS);
}

HAL_StatusTypeDef FRAM::transmitReceive(
    uint8_t* txData,
    uint8_t* rxData,
    uint16_t size
) {
    return HAL_SPI_TransmitReceive(_hspi, txData, rxData, size, SPI_TIMEOUT_MS);
}

FRAM::DeviceID FRAM::readDeviceId() {
    DeviceID id = {0};

    uint8_t command = CMD_RDID;
    uint8_t rx[4] = {0};

    select();

    transmit(&command, 1);
    receive(rx, 4);

    deselect();

    id.manufacturerId   = rx[0];
    id.continuationCode = rx[1];
    id.productId1       = rx[2];
    id.productId2       = rx[3];

    return id;
}

uint8_t FRAM::readStatus() {
    uint8_t tx[2] = {
        CMD_RDSR,
        0x00
    };

    uint8_t rx[2] = {0};

    select();

    transmitReceive(tx, rx, 2);

    deselect();

    return rx[1];
}

void FRAM::writeEnable() {
    uint8_t command = CMD_WREN;

    select();

    transmit(&command, 1);

    deselect();
}

void FRAM::writeDisable() {
    uint8_t command = CMD_WRDI;

    select();

    transmit(&command, 1);

    deselect();
}

void FRAM::writeBuffer(uint32_t address, const uint8_t* data, uint16_t size) {
    writeEnable();

    uint8_t* tx = new uint8_t[4 + size];
    tx[0] = CMD_WRITE;
    tx[1] = static_cast<uint8_t>((address >> 16) & 0xFF);
    tx[2] = static_cast<uint8_t>((address >> 8) & 0xFF);
    tx[3] = static_cast<uint8_t>(address & 0xFF);
    memcpy(&tx[4], data, size);

    select();

    transmit(tx, 4 + size);

    deselect();

    delete[] tx;

    writeDisable();
}

void FRAM::readBuffer(uint32_t address, uint8_t* data, uint16_t size) {
    uint8_t* tx = new uint8_t[4 + size];
    tx[0] = CMD_READ;
    tx[1] = static_cast<uint8_t>((address >> 16) & 0xFF);
    tx[2] = static_cast<uint8_t>((address >> 8) & 0xFF);
    tx[3] = static_cast<uint8_t>(address & 0xFF);
    memset(&tx[4], 0x00, size);

    uint8_t* rx = new uint8_t[4 + size];

    select();

    transmitReceive(tx, rx, 4 + size);

    deselect();

    memcpy(data, &rx[4], size);

    delete[] tx;
    delete[] rx;
}

uint8_t FRAM::read8(uint32_t address) {
    uint8_t tx[5] = {
        CMD_READ,
        static_cast<uint8_t>((address >> 16) & 0xFF),
        static_cast<uint8_t>((address >> 8) & 0xFF),
        static_cast<uint8_t>(address & 0xFF),
        0x00
    };

    uint8_t rx[5] = {0};

    select();

    transmitReceive(tx, rx, 5);

    deselect();

    return rx[4];
}

void FRAM::write8(uint32_t address, uint8_t value) {
    writeEnable();

    uint8_t tx[5] = {
        CMD_WRITE,
        static_cast<uint8_t>((address >> 16) & 0xFF),
        static_cast<uint8_t>((address >> 8) & 0xFF),
        static_cast<uint8_t>(address & 0xFF),
        value
    };

    select();

    transmit(tx, 5);

    deselect();

    writeDisable();
}

bool FRAM::testWriteRead(uint32_t address, uint8_t value) {
    write8(address, value);

    uint8_t readBack = read8(address);

    return readBack == value;
}