#include <stm32l4/NucleoL476rgLowLevel.hpp>

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_utils.h"

namespace {

constexpr std::uint32_t kSystemClockHz = 80000000U;
constexpr std::uint32_t kUartBaudRate  = 115200U;
volatile std::uint32_t  system_tick_ms = 0U;

void configure_clock() {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    while (LL_PWR_IsActiveFlag_VOS() != 0U) {
    }

    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_4WS);
    while (READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_4WS) {
    }

    LL_RCC_HSI_Enable();
    while (LL_RCC_HSI_IsReady() != 1U) {
    }
    LL_RCC_PLL_Disable();
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 10U, LL_RCC_PLLR_DIV_2);
    LL_RCC_PLL_EnableDomain_SYS();
    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1U) {
    }

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
    }

    SystemCoreClock = kSystemClockHz;
    LL_InitTick(kSystemClockHz, 1000U);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
}

void configure_gpio_and_uart() {
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE5, GPIO_MODER_MODE5_0);
    CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT5);
    CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD5);
    CLEAR_BIT(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED5);
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);

    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE2 | GPIO_MODER_MODE3,
               GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);
    CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);
    CLEAR_BIT(GPIOA->PUPDR, GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
    MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3,
               GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3);
    MODIFY_REG(GPIOA->AFR[0], GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3,
               (7U << GPIO_AFRL_AFSEL2_Pos) | (7U << GPIO_AFRL_AFSEL3_Pos));

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    LL_USART_Disable(USART2);
    USART2->CR1 = 0U;
    USART2->CR2 = 0U;
    USART2->CR3 = 0U;
    USART2->BRR = kSystemClockHz / kUartBaudRate;
    SET_BIT(USART2->CR1, USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
    while (LL_USART_IsActiveFlag_TEACK(USART2) == 0U || LL_USART_IsActiveFlag_REACK(USART2) == 0U) {
    }
}

}  // namespace

namespace stm32l4::nucleo_l476rg::low_level {

void initialize() {
    configure_clock();
    configure_gpio_and_uart();
}

void systick_interrupt_handler() {
    ++system_tick_ms;
}

std::uint32_t now_ms() {
    return system_tick_ms;
}

bool uart2_write(const std::uint8_t* data, std::size_t size, std::uint32_t timeout_ms) {
    if (data == nullptr && size != 0U) {
        return false;
    }

    const std::uint32_t start_ms = now_ms();
    for (std::size_t index = 0U; index < size; ++index) {
        while (LL_USART_IsActiveFlag_TXE(USART2) == 0U) {
            if (static_cast<std::uint32_t>(now_ms() - start_ms) >= timeout_ms) {
                return false;
            }
        }
        LL_USART_TransmitData8(USART2, data[index]);
    }
    return true;
}

bool uart2_try_read(std::uint8_t& data) {
    if (LL_USART_IsActiveFlag_RXNE(USART2) == 0U) {
        return false;
    }
    data = LL_USART_ReceiveData8(USART2);
    return true;
}

void ld2_set(bool is_on) {
    if (is_on) {
        LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
    } else {
        LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
    }
}

bool ld2_is_on() {
    return LL_GPIO_IsOutputPinSet(GPIOA, LL_GPIO_PIN_5) != 0U;
}

}  // namespace stm32l4::nucleo_l476rg::low_level
