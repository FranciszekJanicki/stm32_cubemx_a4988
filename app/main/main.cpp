#include "main.h"
#include "a4988.hpp"
#include "gpio.h"
#include "usart.h"
#include <cstdio>

int main()
{
    HAL_Init();
    SystemClock_Config();

    MX_USART2_UART_Init();
    MX_GPIO_Init();

    using namespace A4988;

    auto constexpr MS1 = GPIO::PA0;
    auto constexpr MS2 = GPIO::PA1;
    auto constexpr MS3 = GPIO::PA2;
    auto constexpr RESET = GPIO::PA3;
    auto constexpr SLEEP = GPIO::PA4;
    auto constexpr DIR = GPIO::PA5;
    auto constexpr ENABLE = GPIO::PA6;

    auto pwm_device = PWMDevice{};

    auto a4988 = A4988::A4988{std::move(pwm_device), MS1, MS2, MS3, RESET, SLEEP, DIR, ENABLE};

    while (true) {
    }

    return 0;
}
