#include "nrf.h"
#define BUTTON1 11
#define BUTTON2 12
#define BUTTON3 24
#define BUTTON4 25


int main(void)
{
    NRF_P0->PIN_CNF[BUTTON1] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
    NRF_P0->PIN_CNF[BUTTON2] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
    NRF_P0->PIN_CNF[BUTTON3] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
    NRF_P0->PIN_CNF[BUTTON4] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) | (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
    NRF_P0->DIRSET = GPIO_DIRSET_PIN13_Msk | GPIO_DIRSET_PIN14_Msk | GPIO_DIRSET_PIN15_Msk | GPIO_DIRSET_PIN16_Msk;

    while (1)
    {
        if (!(NRF_P0->IN & GPIO_IN_PIN11_Msk))
        {
            NRF_P0->OUTCLR = GPIO_OUTCLR_PIN13_Msk;
        }
        else
        {
            NRF_P0->OUTSET = GPIO_OUTSET_PIN13_Msk;
        }
        if (!(NRF_P0->IN & GPIO_IN_PIN12_Msk))
        {
            NRF_P0->OUTCLR = GPIO_OUTCLR_PIN14_Msk;
        }
        else
        {
            NRF_P0->OUTSET = GPIO_OUTSET_PIN14_Msk;
        }
        if (!(NRF_P0->IN & GPIO_IN_PIN24_Msk))
        {
            NRF_P0->OUTCLR = GPIO_OUTCLR_PIN15_Msk;
        }
        else
        {
            NRF_P0->OUTSET = GPIO_OUTSET_PIN15_Msk;
        }
        if (!(NRF_P0->IN & GPIO_IN_PIN25_Msk))
        {
            NRF_P0->OUTCLR = GPIO_OUTCLR_PIN16_Msk;
        }
        else
        {
            NRF_P0->OUTSET = GPIO_OUTSET_PIN16_Msk;
        }
    }
}