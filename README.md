# STM32F030-CMSIS-PWM-Blinky
+ **main.c** shows how to set up PWM to blink an LED connected to pin 13.
+ **main-plus-advanced-timer.c** demonstrates how to implement PWM using an advanced timer by also blinking LEDs connected to pins 14 and 18.
+ Be sure to use approx. 1 k ohm current-limiting resistors when connecting LEDs to the microcontroller!<br>
+ See code for details.

## To use main-plus-advanced-timer.c:
1. Rename **main.c** to **main.c.old**
2. Rename **main-plus-advainced-timer.c** to **main.c**
3. Run ```make clean && make```

## Basic Steps to Set Up PWM
(Also outlined in **main.c**)
1. Enable GPIO Port A by setting the RCC_APBENR_GPIOAEN bit in the RCC AHBENR register.
2. Enable TIMER 3 by setting the RCC_APB1ENR_TIM3EN bit in the RCC APB1ENR register.
3. Set GPIOA7 to alternate function mode by changing the GPIOA MODER7 bits to 0b10 in
   the GPIOA MODER register.
4. Set the alternate function of A7 to Timer 3, Channel 2, which is alternate function 1.
   Do this by setting the AFSEL7[3:0] bits in the GPIOA AFR[0] (actually AFRL in the
   datasheet) to 1.
5. Set the Timer 3 Channel 2 output compare mode to PWM Mode 1 by setting the
   OC2M[2:0] bits in the TIM3 CCMR1 register to 0b110.
6. Set the prescaler via the TIM3 PSC register. A prescaler of 999 will divide the clock
   by 1000, making the ARR period counter units into 1/8000 s.
7. Set PWM period (and, thus, the frequency) via the TIM3 ARR register. With a prescaler
   of 1000, a period of 8000 ticks will equal 1 s.
8. Set PWM duty cycle via the TIM3 CCR2 register. A setting of 4000 means the duty cycle
   would be 4000/8000 = 50%.
9. Enable output Timer 3 Channel 2 to be output to the GPIO pin by setting the
   TIM_CCER_CC2E bit in the TIM3 CCER register.
10. Enable the clock counter via the TIM_CR1_CEN bit in the TIM2 CR1 register.
