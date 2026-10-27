#include "nrf.h"
#define LED1 13
#define LED1_Msk GPIO_OUTSET_PIN13_Msk

int main(void)
{
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
    }

    NRF_P0->PIN_CNF[LED1] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos)
                          | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos);
    NRF_TIMER0->MODE = (TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos);
    NRF_TIMER0->BITMODE = (TIMER_BITMODE_BITMODE_24Bit << TIMER_BITMODE_BITMODE_Pos);
    NRF_TIMER0->PRESCALER = (4 << TIMER_PRESCALER_PRESCALER_Pos);
    NRF_TIMER0->CC[0] = 500000;
    NRF_TIMER0->SHORTS = (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);
    NRF_TIMER0->TASKS_START = 1;

    while (1)
    {
        while (NRF_TIMER0->EVENTS_COMPARE[0] == 0)
        {
        }
        NRF_TIMER0->EVENTS_COMPARE[0] = 0;
        if (NRF_P0->OUT & LED1_Msk)
            NRF_P0->OUTCLR = LED1_Msk;
        else
            NRF_P0->OUTSET = LED1_Msk;
    }
}