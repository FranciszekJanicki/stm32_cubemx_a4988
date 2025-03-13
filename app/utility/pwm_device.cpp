#include "pwm_device.hpp"

namespace Utility {

    PWMDevice::PWMDevice(TIMHandle const timer,
                         std::uint16_t const channel_mask,
                         std::uint16_t const counter_period,
                         float const ref_voltage) noexcept :
        timer_{timer}, channel_mask_{channel_mask}, min_raw_{0U}, max_raw_{counter_period}, ref_voltage_{ref_voltage}
    {
        this->initialize();
    }

    PWMDevice::PWMDevice(TIMHandle const timer,
                         std::uint16_t const channel_mask,
                         std::uint16_t const min_raw,
                         std::uint16_t const max_raw,
                         float const ref_voltage) noexcept :
        timer_{timer}, channel_mask_{channel_mask}, min_raw_{min_raw}, max_raw_{max_raw}, ref_voltage_{ref_voltage}
    {
        this->initialize();
    }

    PWMDevice::~PWMDevice() noexcept
    {
        this->deinitialize();
    }

    void PWMDevice::set_compare_raw(std::uint16_t const raw) const noexcept
    {
        if (this->initialized_) {
            std::printf("DUTY: %.2f%%\n\r",
                        static_cast<float>(raw) / static_cast<float>(this->timer_->Init.Period) * 100.0F);
            __HAL_TIM_SetCompare(this->timer_, this->channel_mask_, raw);
        }
    }

    void PWMDevice::set_compare_voltage(float const voltage) const noexcept
    {
        this->set_compare_raw(this->voltage_to_raw(voltage));
    }

    void PWMDevice::set_compare_max() const noexcept
    {
        this->set_compare_raw(this->max_raw_);
    }

    void PWMDevice::set_compare_min() const noexcept
    {
        this->set_compare_raw(this->min_raw_);
    }

    float PWMDevice::get_ref_voltage() const noexcept
    {
        return this->ref_voltage_;
    }

    void PWMDevice::set_frequency(std::uint32_t const frequency) noexcept
    {
        if (frequency > 0UL) {
            this->deinitialize();

            std::uint32_t constexpr CLOCK_HZ = 80000000UL;
            std::uint32_t prescaler = 0UL;
            std::uint32_t counter_period = CLOCK_HZ / frequency;
            while (counter_period > 0xFFFF) {
                prescaler++;
                counter_period = CLOCK_HZ / ((prescaler + 1UL) * frequency) - 1UL;
            }
            this->timer_->Init.Prescaler = prescaler;
            this->timer_->Init.Period = counter_period;
            printf("FREQ: %lu hz, PSC: %lu, CP: %lu\n\r", frequency, prescaler, counter_period);

            this->initialize();
        }
    }

    void PWMDevice::initialize() noexcept
    {
        if (this->timer_ != nullptr) {
            if (HAL_TIM_PWM_Start(this->timer_, this->channel_mask_) == HAL_OK) {
                this->initialized_ = true;
            }
        }
    }

    void PWMDevice::deinitialize() noexcept
    {
        if (this->timer_ != nullptr) {
            if (HAL_TIM_PWM_Stop(this->timer_, this->channel_mask_) == HAL_OK) {
                this->initialized_ = false;
            }
        }
    }

    std::uint16_t PWMDevice::voltage_to_raw(float const voltage) const noexcept
    {
        return Utility::rescale(voltage, 0.0F, this->ref_voltage_, this->min_raw_, this->max_raw_);
    }

    float PWMDevice::raw_to_voltage(std::uint16_t const raw) const noexcept
    {
        return Utility::rescale(raw, this->min_raw_, this->max_raw_, 0.0F, this->ref_voltage_);
    }

}; // namespace Utility
