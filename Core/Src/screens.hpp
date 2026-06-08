#ifndef SCREENS_HPP
#define SCREENS_HPP
extern "C" {
#include "ugui.h"
}
#ifdef swap
#undef swap
#endif
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include "../Inc/magic_enum.hpp"

namespace Display::Dimensions{
    constexpr uint16_t kHorizontalResolution = 320;
    constexpr uint16_t kVerticalResolution = 240;
    constexpr uint16_t kTopBarVerticalSize = 28;
    constexpr uint16_t kStatusCellY = kTopBarVerticalSize + 1;
    constexpr uint16_t kStatusCellHeight = 55;
    constexpr uint16_t kStatusCellBottomMargin = 5;
    constexpr uint16_t kValueCellSideMargin = 5;
    constexpr uint16_t kValueCellTop = kStatusCellY + kStatusCellHeight + kStatusCellBottomMargin;
    constexpr uint16_t kValueCellWidth = (kHorizontalResolution - (2 * kValueCellSideMargin)) / 2;
    constexpr uint16_t kValueCellHeight = 40;
    constexpr uint16_t kValueCellRowGap = 0;
    constexpr uint16_t kValueCellSecondRowTop = kValueCellTop + kValueCellHeight + kValueCellRowGap;
    constexpr uint16_t kValueCellThirdRowTop = kValueCellSecondRowTop + kValueCellHeight + kValueCellRowGap;
}
namespace Display::DesignHelpers {
    #define FONT_SMALL     FONT_6X8
    #define FONT_NORMAL    FONT_8X12
    #define FONT_BIG       FONT_12X16
    #define FONT_HUGE      FONT_16X26

    // NOT RGB!: Green = RED, BLUE=GREEN, RED=BLUE
    #define RGB565(r,g,b) (UG_COLOR)((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))
    UG_COLOR kErrorColor = RGB565(0, 255, 0); // Red
    UG_COLOR kWarningColor = RGB565(0,255, 255); // Yellow
    UG_COLOR kSuccessColor = RGB565(0, 0, 255); // Green
    UG_COLOR kStatusTextColor = RGB565(0, 0, 0); // Black
    UG_COLOR kBackgroundColor = RGB565(255, 255, 255); // White
    UG_COLOR kTextColor = RGB565(0, 0, 0); // Black
    UG_COLOR kFrameColor = 0xFBDE; // Light grey

    class TopBar {
        private:
        std::string kTitle;
        UG_COLOR kBackgroundColor = RGB565(255, 255, 255); // White
        UG_COLOR kTextColor = RGB565(0, 0, 0); // Black

        public:
        TopBar(std::string title) : kTitle(title) {}
        void draw()
        {
            UG_FillFrame(0, 0, Display::Dimensions::kHorizontalResolution, Display::Dimensions::kTopBarVerticalSize, kBackgroundColor);
            UG_DrawLine(0, 28, Display::Dimensions::kHorizontalResolution, Display::Dimensions::kTopBarVerticalSize, kTextColor);
            UG_FontSelect(FONT_12X16);
            UG_SetForecolor(kTextColor);
            UG_SetBackcolor(kBackgroundColor);
            UG_PutString(124, 5, (char *)kTitle.c_str());
        }
    };

    class StatusCell{
        public:
        StatusCell(std::string title) : kTitle(title) {}

        void draw(UG_COLOR statusColor){
            UG_FillFrame(0, Display::Dimensions::kStatusCellY,
                         Display::Dimensions::kHorizontalResolution,
                         Display::Dimensions::kStatusCellY + Display::Dimensions::kStatusCellHeight,
                         statusColor);
            UG_FontSelect(FONT_16X26);
            UG_SetForecolor(kStatusTextColor);
            UG_SetBackcolor(statusColor);
            UG_PutString(30, Display::Dimensions::kStatusCellY + 10, (char *)kTitle.c_str());
        }
        
        private:
            std::string kTitle;
    };
    
    template<class T>
    class ValueCell {
        private:
        std::string name;
        T value;
        uint16_t kAnchorX = 8;
        uint16_t kAnchorY = 6;
        uint16_t kSizeX = 115;
        uint16_t kSizeY = 40;
        public:
        void setValue(T newValue){
            value = newValue;
        }
        ValueCell() = default;
        ValueCell(std::string name, T value, uint16_t anchorX, uint16_t anchorY, uint16_t sizeX, uint16_t sizeY) : name(name), value(value), kAnchorX(anchorX), kAnchorY(anchorY), kSizeX(sizeX), kSizeY(sizeY) {}
        void draw(){
            UG_FillFrame(kAnchorX, kAnchorY, kAnchorX + kSizeX, kAnchorY + kSizeY, kBackgroundColor);
            UG_DrawFrame(kAnchorX, kAnchorY, kAnchorX + kSizeX, kAnchorY + kSizeY, kFrameColor);
            UG_FontSelect(FONT_8X12);
            UG_SetForecolor(kTextColor);
            UG_SetBackcolor(kBackgroundColor);
            UG_PutString(kAnchorX + 8, kAnchorY + 6, (char *)name.c_str());
            UG_PutString(kAnchorX + 8, kAnchorY + 24, (char *)std::to_string(value).c_str()); // Assume int
        }

    };
}

namespace Display::Screens {

class BasicScreen {
private:
    std::string kTitle;
protected:
    std::string kStatus;
    std::array<DesignHelpers::ValueCell<int>, 10> kValueCells; // Assuming a maximum of 10 value cells
    uint8_t kValueCellCount = 0;
    UG_COLOR kStatusColor = DesignHelpers::kSuccessColor;
public:
    bool addValueCell(DesignHelpers::ValueCell<int> cell){
        if (kValueCellCount < kValueCells.size()) {
            kValueCells[kValueCellCount++] = cell;
            return true;
        }
        return false;
    }
    void setStatusColor(UG_COLOR color){
        kStatusColor = color;
    }

    void draw(){
        UG_FillScreen(DesignHelpers::kBackgroundColor);
        DesignHelpers::TopBar topBar(kTitle);
        topBar.draw();
        DesignHelpers::StatusCell kStatusCell(kStatus);
        kStatusCell.draw(kStatusColor);
        for (uint8_t i = 0; i < kValueCellCount; ++i) {
            kValueCells[i].draw();
        }
    }
    BasicScreen(std::string title, std::string status) : kTitle(title), kStatus(status) {};
};

    struct StatusData {
        uint32_t controlWord;
        uint32_t statusWord;
        uint32_t mode;
        uint32_t rpm;
        uint32_t target;
        uint32_t response;
    };
class StatusScreen : public BasicScreen {

public:
    StatusScreen(std::string title, std::string status) : BasicScreen(title, status) { // Value Cells should begin below Statuscell. Place two ValueCells next to each other
        addValueCell(DesignHelpers::ValueCell<int>("Controword", 0,
                                                   Dimensions::kValueCellSideMargin,
                                                   Dimensions::kValueCellTop,
                                                   Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellHeight)); // First row:
        addValueCell(DesignHelpers::ValueCell<int>("Statusword", 0,
                                                   Dimensions::kValueCellSideMargin + Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellTop,
                                                   Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellHeight));
        addValueCell(DesignHelpers::ValueCell<int>("Mode", 0,
                                                   Dimensions::kValueCellSideMargin,
                                                   Dimensions::kValueCellSecondRowTop,
                                                   Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellHeight));
        addValueCell(DesignHelpers::ValueCell<int>("RPM", 0,
                                                   Dimensions::kValueCellSideMargin + Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellSecondRowTop,
                                                   Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellHeight));

        addValueCell(DesignHelpers::ValueCell<int>("Target", 0,
                                                   Dimensions::kValueCellSideMargin,
                                                   Dimensions::kValueCellThirdRowTop,
                                                   Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellHeight));
        addValueCell(DesignHelpers::ValueCell<int>("Response", 0,
                                                   Dimensions::kValueCellSideMargin + Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellThirdRowTop,
                                                   Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellHeight));
    }
    void updateValues(StatusData data){
        kValueCells[0].setValue(data.controlWord);
        kValueCells[1].setValue(data.statusWord);
        kValueCells[2].setValue(data.mode);
        kValueCells[3].setValue(data.rpm);
        kValueCells[4].setValue(data.target);
        kValueCells[5].setValue(data.response);
    }
    void updateStatus(cia402::statemachine::DriveState drive_state){
        switch (drive_state){
            case cia402::statemachine::DriveState::kFaultReactionActive:
            setStatusColor(DesignHelpers::kErrorColor);
            break;
            
            case cia402::statemachine::DriveState::kFault:
            setStatusColor(DesignHelpers::kErrorColor);
            break;
            
            case cia402::statemachine::DriveState::kOperationEnabled:
            setStatusColor(DesignHelpers::kSuccessColor);
            break;
            
            default:
            setStatusColor(DesignHelpers::kWarningColor);
            break;
        }
        kStatus.assign(magic_enum::enum_name(drive_state));
    }
    
};

class NetworkScreen : public BasicScreen {

};

struct SensorData {
    float temperature;
    float humidity;
};

class HardwareScreen : public BasicScreen {
public:
HardwareScreen(std::string title, std::string status) : BasicScreen(title, status) {
        addValueCell(DesignHelpers::ValueCell<int>("Temperature", 0,
                                                   Dimensions::kValueCellSideMargin,
                                                   Dimensions::kValueCellTop,
                                                   Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellHeight)); // First row:
        addValueCell(DesignHelpers::ValueCell<int>("Humidity", 0,
                                                   Dimensions::kValueCellSideMargin + Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellTop,
                                                   Dimensions::kValueCellWidth,
                                                   Dimensions::kValueCellHeight));
    }
    auto updateValues(SensorData data) -> void {
        kValueCells[0].setValue(static_cast<int>(data.temperature));
        kValueCells[1].setValue(static_cast<int>(data.humidity));
    }
private:
    bool temperature_sensor_alive = false;
    float temperature = 0.0;
    float humidity = 0.0;
};

class SettingsScreen : public BasicScreen {

};

class ErrorScreen : public BasicScreen {

};

class SDOScreen : public BasicScreen {

};

class PDOScreen : public BasicScreen {

};
}
#endif // SCREENS_HPP
