#include <string>
#include <cstdint>
#include <utility>
#include "Kinematics.hpp"
extern "C"
{
    #include "spi.h"
    #include "gpio.h"
}

namespace config{
    template<class Backend> class ObjectDictionary{

        public: 
        explicit ObjectDictionary(std::string path)
            : backend_(std::move(path))
        {
        }

        explicit ObjectDictionary(SPI_HandleTypeDef* hspi,
            GPIO_TypeDef* csPort,
            uint16_t csPin)
            : backend_(hspi, csPort, csPin)
        {
        }
        auto Restore() -> void
        {
            for(auto& entry : motion::persisted_data)
            {
                entry.value = RestoreEntry(entry);
            }
        }

        auto Store() -> bool
        {
            for(const auto& entry : motion::persisted_data)
            {
                if(!StoreEntry(entry))
                {
                    return false;
                }
            }
            return true;
        }

        private:         
        auto RestoreEntry(const motion::PersistedData& entry) -> uint32_t
        {
            return backend_.template Read<uint32_t>(entry.address);
        }

        auto StoreEntry(const motion::PersistedData& entry) -> bool
        {
            return backend_.template Write<uint32_t>(entry.address, entry.value);
        }
        Backend backend_;

    };
}
