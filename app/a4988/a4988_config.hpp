#ifndef A4988_CONFIG_HPP
#define A4988_CONFIG_HPP

#include "../utility/gpio.hpp"
#include "../utility/pwm_device.hpp"

namespace A4988 {

    enum struct Microstep : std::uint8_t {
        FULL,
        HALF,
        QUARTER,
        EIGHTH,
        SIXTEENTH,
    };

    enum struct Direction : std::uint8_t {
        FORWARD,
        BACKWARD,
    };

    using namespace Utility;

    inline float microstep_to_fraction(Microstep const microstep) noexcept
    {
        switch (microstep) {
            case Microstep::FULL:
                return 1.0F;
            case Microstep::HALF:
                return 0.5F;
            case Microstep::QUARTER:
                return 0.25F;
            case Microstep::EIGHTH:
                return 0.125F;
            case Microstep::SIXTEENTH:
                return 0.0625F;
            default:
                return 0.0F;
                break;
        }
    }

    auto constexpr PULSE_WIDTH_RAW = 100U;

} // namespace A4988

#endif // A4988_CONFIG_HPP