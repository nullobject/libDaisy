#pragma once
#ifndef DSY_HALLSENSOR_H
#define DSY_HALLSENSOR_H

#include "stm32h7xx_hal.h"
extern "C"
{
#include "util/hal_map.h"
}

namespace daisy
{
/** @addtogroup external 
    @{ 
*/

/** @brief Device support for US5881 Hall Effect Sensor
    @author beserge
    @date December 2021
*/
class HallSensor
{
  public:
    HallSensor() {}
    ~HallSensor() {}

    struct Config
    {
        enum class Peripheral
        {
            TIM_1,
            TIM_2,
            TIM_3,
            TIM_4,
            TIM_5,
            TIM_6
        };

        Peripheral periph;

        uint32_t polarity;
        uint32_t prescaler;
        uint32_t filter;
        uint32_t commutation_delay;

        dsy_gpio_pin pin;

        Config()
        {
            periph = Peripheral::TIM_3;
            pin    = {DSY_GPIOA, 8};

            polarity
                = TIM_ICPOLARITY_RISING; // options are rising, falling, bothedge
            prescaler = TIM_ICPSC_DIV1;  // options are 1, 2, 4, 8
            filter    = 0x05; // range is 0x0 -> 0xF basically the debounce
            commutation_delay = 0x0000; // range is 0x0000 -> 0xFFFF
        }
    };

    /** Initialize the Hall Sensor device
        \param config Configuration settings
    */
    void Init(Config config);

    /** Start reading in a blocking fashion. Init calls this by default. */
    void StartBlockingRead();

    /** Stop reading in a blocking fashion */
    void StopBlockingRead() { HAL_TIMEx_HallSensor_Stop(&hall_); }

    /** Get the total count of hall sensor clicks so far 
        \return Total number of times hall sensor has gone high since init.
    */
    uint8_t GetCount() { return hall_.Instance->CNT; }

  private:
    Config            config_;
    TIM_HandleTypeDef hall_;

    /** Get the hall sensor pin going and working with the correct clock */
    void InitPins();

    /** Deinit the hall sensor pin. Unused as of now. */
    void DeInitPins();

    /** @} */
}; // HallSensor

} // namespace daisy
#endif