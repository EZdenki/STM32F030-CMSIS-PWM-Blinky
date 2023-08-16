//  main-plus-advanced-timer.c
//    Multi-channel PWM Blinky with only CMSIS (no HAL)
//
//    Mike Shegedin, EZdenki.com
//
//    Version 1.0     16 Aug 2023   Cleaned up comments
//    Version 0.9        Jul 2023   Started
//
//  This project will build upon STM32F030-PWM-Blinky main.c in order to demonstrate how to
//  use the Advanced Timer by adding Advanced Timer 1 Channel 3 and the complement of Timer 1
//  Channel 3, which is Timer 1 Channel 3N. 
//
//  To Build:
//  =========
//  To use this program, rename main.c to main.c.old, and then rename this file to main.c,
//  and then use "make clean && make" to build as normal.
//
//  Hardware Setup:
//  ===============
//
//  Attach LEDs and current-limiting resistors to the following pins:
//  /===========/=========================/=========/======/=============
//  | STM32F030 |      Timer Number       |  Timer  | GPIO | Alternate  |
//  |    Pin    |        and Type         | Channel |  Pin |  Function  |
//  |-----------|-------------------------|---------|------|------------|
//  |    13     | General Purpose Timer 3 |  Ch 2   |  A7  | Alt Func 1 |
//  |    18     |     Advanced Timer 1    |  Ch 3   |  A10 | Alt Func 2 |
//  |    14     |     Advanced Timer 1    |  Ch 3N  |  B1  | Alt Func 2 |
//  /===========/=========================/=========/======/=============


#include "stm32f030x6.h"


int main( void ){

  // 1. Enable GPIO Port A and Port B
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Tim3_Ch2_PA7_AF1_Pin13 / Tim1_Ch3_PA10_AF2_Pin18
  RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // Tim1_Ch3N_PB1_AF2_Pin14

  // 2. Enable Timer 3 and 1
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // Tim3_Ch2_PA7_AF1
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // Tim1_Ch3_PA10_AF2 / Tim1_Ch3N_PB1_AF2

  // 3. Set MODERx[1:0] bits to 0b10 to enable the alternate function mode for these pins.
  GPIOA->MODER |= ( GPIO_MODER_MODER7_1);  // Tim3_Ch2_PA7_AF1
  GPIOA->MODER |= ( GPIO_MODER_MODER10_1); // Tim1_Ch3_PA10_AF2
  GPIOB->MODER |= ( GPIO_MODER_MODER1_1);  // Tim1_Ch3N_PB1_AF2
    
  // 4. Set the 1st alternate function of pin A7 and the 2nd alternate function for pins
  //    A10 and B1.
  GPIOA->AFR[0] |= (1<<GPIO_AFRL_AFSEL7_Pos);  // Tim3_Ch2_PA7_AF1
  GPIOA->AFR[1] |= (2<<GPIO_AFRH_AFSEL10_Pos); // Tim1_Ch3_PA10_AF2
  GPIOB->AFR[0] |= (2<<GPIO_AFRL_AFSEL1_Pos);  // Tim1_Ch3N_PB1_AF2

  // 5. Set TIM3/CH2 and TIM1/CH3 output compare mode to PWM Mode 1 (OCxM[2:0] = 0b110)
  TIM3->CCMR1 |= (0b110 << TIM_CCMR1_OC2M_Pos); //Tim3_Ch2_PA7_AF1
  TIM1->CCMR2 |= (0b110 << TIM_CCMR2_OC3M_Pos); //Tim1_Ch3(N)_PA10/PB1_AF2

  // 6. Set prescaler to divide clock by 1000
  TIM3->PSC = 1000; // Tim1_Ch1_PA7_AF1
  TIM1->PSC = 1000; // Tim1_Ch3(N)_PA10/PB1_AF2
  
  // 7. Set TIM3 Frequency to 4000 (8 MHz clock / 4000 = 500 Hz output when there is no
  //    prescaler) and TIM1 frequency to 8000 (1 kHz output when there is no prescaler.)
  TIM3->ARR = 4000; // Tim3_Ch2_PA7_AF1
  TIM1->ARR = 8000; // Tim1_Ch3(N)_PA10/PB1_AF2

  // 8. Set TIM1/CH3 Duty Cycle to 25%/50% (25% of 4000 = 1000)
  TIM3->CCR2 = 1000; // Tim3_Ch2_PA7_AF1
  TIM1->CCR3 = 4000; // Tim1_Ch3(N)_PA10/PB1_AF2

  // 9. Enable output on Timer Channel x output pins
  TIM3->CCER |= TIM_CCER_CC2E;  // Tim3_Ch2_PA7_AF1
  TIM1->CCER |= TIM_CCER_CC3E;  // Tim1_Ch3_PA10_AF2
  TIM1->CCER |= TIM_CCER_CC3NE; // Tim1_Ch3N_PB1_AF2

  // 10. Finally, enable the clock counters for Timer 3 and Timer 1 (turn on the clock!)
  TIM3->CR1 |= TIM_CR1_CEN; // Tim3_Ch2_PA7_AF1
  TIM1->CR1 |= TIM_CR1_CEN; // Tim1_Ch3(N)_PA10/PB1_AF2

  // 10.5 The MOE bit needs to be set to enabout output on Advanced Timer
  TIM1->BDTR |= TIM_BDTR_MOE;

  // Endless Loop
  while( 1 ) ;

  return 0;
}
