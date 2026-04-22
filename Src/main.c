#include "stm32f108.h"
void Set_Motor_Speed(uint8_t motor, uint16_t speed)
{
    uint16_t duty = 0;
    uint8_t dir = 0;
    if (speed > 0)
    {
        duty = speed;
        dir = 1;
    }
    else if (speed < 0)
    {
        duty = -speed;
        dir = 2;
    }
    else
    {
        duty = 0;
        dir = 0;
    }
    if (duty > 999)
        duty = 999;
    if (motor == 1)
    {
        if (dir == 1)
        {
            GPIOB->BSRR = (1 << 4);
            GPIOB->BRR = (1 << 5);
        }
        else if (dir == 2)
        {
            GPIOB->BRR = (1 << 4);
            GPIOB->BSRR = (1 << 5);
        }
        else
        {
            GPIOB->BRR = (1 << 4);
            GPIOB->BRR = (1 << 5);
        }
        TIM2->CCR2 = duty;
    }
    else if (motor == 2)
    {
        if (dir == 1)
        {
            GPIOB->BSRR = (1 << 6);
            GPIOB->BRR = (1 << 7);
        }
        else if (dir == 2)
        {
            GPIOB->BRR = (1 << 6);
            GPIOB->BSRR = (1 << 7);
        }
        else
        {
            GPIOB->BRR = (1 << 6) | (1 << 7);
        }
        TIM2->CCR3 = duty;
    }
}
// Foward
void move_forward(int16_t speed)
{
    Set_Motor_Speed(1, speed);
    Set_Motor_Speed(2, speed);
}

// Backward
void move_backward(int16_t speed)
{
    Set_Motor_Speed(1, -speed);
    Set_Motor_Speed(2, -speed);
}

// Turn Left
void turn_left(int16_t speed)
{
    Set_Motor_Speed(1, -speed);
    Set_Motor_Speed(2, speed);
}

// Turn Right
void turn_right(int16_t speed)
{
    Set_Motor_Speed(1, speed);
    Set_Motor_Speed(2, -speed);
}
// Stop
void stop_robot()
{
    Set_Motor_Speed(1, 0);
    Set_Motor_Speed(2, 0);
}
void delay(volatile uint32_t count)
{
    while (count--)
    {
    }
}
int main(void)
{
    RCC->APB2ENR |= (1 << 2) | (1 << 3) | (1 << 0);
    RCC->APB1ENR |= (1 << 0);
    AFIO->MAPR |= (0x2 << 24);
    GPIOA->CRL &= ~(0xF << 8);
    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |= (0xB << 8) | (0xB << 4);
    GPIOB->CRL &= ~((0xF << 16) | (0xF << 20) | (0xF << 24) | (0xF << 28));
    GPIOB->CRH &= ~(0xF << 1);
    GPIOB->CRL |= (0x1 << 16) | (1 << 20) | (0x1 << 24) | (0x1 << 28);
    GPIOB->CRH |= (0x1 << 1);
    TIM2->PSC = 7;
    TIM2->ARR = 999;
    TIM2->CCMR1 &= ~(0x7 << 12);
    TIM2->CCMR1 |= (0x6 << 12);
    TIM2->CCMR2 &= ~(0x7 << 4);
    TIM2->CCMR2 |= (0x6 << 4);
    TIM2->CCER |= (1 << 4) | (1 << 8);
    TIM2->CCR2 = 0;
    TIM2->CCR3 = 0;
    TIM2->CR1 |= (1 << 0);
    while (1)
    {
        move_forward(100);
        delay(10000);
        move_backward(100);
        delay(10000);
    }
}