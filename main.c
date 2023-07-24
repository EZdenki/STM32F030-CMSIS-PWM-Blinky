//  Minimalist PWM Blinky with only CMSIS (no HAL)
//  Target: STM32F030F4xx
//  Mike Shegedin, 04/2023
//
//  This projects demostrates how to use PWM to flash an LED. Timer 3, channel 2 will be
//  used to flash an LED attached to GPIO A7 (pin 13 on the STM32F030F4).
//
//  Attach an anode (+ side) of LED to GPIO Pin A7. Then connect a 1k resister between the
//  and cathode (- side) of the LED and ground.

//  Steps Involved. Specific to GPIO:

//  1. Enable GPIO Port A by setting the RCC_APBENR_GPIOAEN bit in the RCC AHBENR register.
//  2. Enable TIMER 3 by setting the RCC_APB1ENR_TIM3EN bit in the RCC APB1ENR register.
//  3. Set GPIOA7 to alternate function mode by changing the GPIOA MODER7 bits to 0b10 in
//     the GPIOA MODER register.
//  4. Set the alternate function of A7 to Timer 3, Channel 2, which is alternate function 1.
//     Do this by setting the AFSEL7[3:0] bits in the GPIOA AFR[0] (actually AFRL in the
//     datasheet) to 1.
//  5. Set the Timer 3 Channel 2 output compare mode to PWM Mode 1 by setting the
//     OC2M[2:0] bits in the TIM3 CCMR1 register to 0b110.
//  6. Set the prescaler via the TIM3 PSC register. A prescaler of 999 will divide the clock
//     by 1000, making the ARR period counter units into 1/8000 s.
//  7. Set PWM period (and, thus, the frequency) via the TIM3 ARR register. With a prescaler
//     of 1000, a period of 8000 ticks will equal 1 s.
//  8. Set PWM duty cycle via the TIM3 CCR2 register. A setting of 4000 means the duty cycle
//     would be 4000/8000 = 50%.
//  9. Enable output Timer 3 Channel 2 to be output to the GPIO pin by setting the
//     TIM_CCER_CC2E bit in the TIM3 CCER register.
//     register.
// 10. Enable the clock counter via the TIM_CR1_CEN bit in the TIM2 CR1 register.

void SystemInit( void );

#include "stm32f030x6.h"
int main( void ){

  // =========================================
  // Setup up PWM Output on GPIO A0 (TIM2/CH1)
  // =========================================

  // 1. Enable GPIO Port A
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Tim3_Ch2_PA7_AF1_Pin13 / Tim1_Ch3_PA10_AF2_Pin18

  // 2. Enable Timer 3 and 1
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Tim3_Ch2_PA7_AF1

  // 3. Set MODER7[1:0] bits1 to enable the alternate function mode for this pin.
  GPIOA->MODER |= ( GPIO_MODER_MODER7_1);  // Tim3_Ch2_PA7_AF1
    
  // 4. Set the alternate function of pin A7 to the first alternate function, which is
  //    Timer 3, channel 2.
  GPIOA->AFR[0] |= (1<<GPIO_AFRL_AFSEL7_Pos);  // Tim3_Ch2_PA7_AF1

  // 5. Set TIM3/CH2 output compare mode to PWM Mode 1 (OC2M[2:0] = 0b110)
  TIM3->CCMR1 |= (0b110 << TIM_CCMR1_OC2M_Pos); //Tim3_Ch2_PA7_AF1

  // 6. Set prescaler to divide clock by 1000
  TIM3->PSC = 1000; // Tim1_Ch1_PA7_AF1
  
  // 7. Set TIM3 Frequency to 8000 (8 MHz clock / 8000 = 1 kHz output when there is no
  //    prescaler)
  TIM3->ARR = 8000; // Tim3_Ch2_PA7_AF1

  // 8. Set TIM3/CH2 Duty Cycle to 50% (50% of 8000 = 4000)
  TIM3->CCR2 = 4000; // Tim3_Ch2_PA7_AF1

  // 9. Enable output on Timer 3 Channel 1 pin
  TIM3->CCER |= TIM_CCER_CC2E;  // Tim3_Ch2_PA7_AF1

  // 10. Finally, enable the clock counter (turn on the clock!)
  TIM3->CR1 |= TIM_CR1_CEN; // Tim3_Ch2_PA7_AF1

  // Endless Loop
  while( 1 )
  {
  }
}
