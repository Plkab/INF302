/**
 * @file delay.c
 * @brief Implémentation des délais avec le Timer 8 (TIM9).
 */

#include "delay.h"
#include "stm32f4xx.h"

void Delay_Init(void) {
    // 1. Activer l'horloge de TIM9 (bus APB2)
    RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;

    // 2. Prescaler : division par 16 pour obtenir 1 MHz
    //    Horloge APB2 = 16 MHz, PSC = 16 ? 16 000 000 / (16+1) = 1 000 000 Hz
    TIM9->PSC = 16 - 1;   // 16

    // 3. Auto-reload : valeur maximale (16 bits)
    TIM9->ARR = 0xFFFF;

    // 4. Remettre le compteur à zéro
    TIM9->CNT = 0;

    // 5. Démarrer le timer
    TIM9->CR1 |= TIM_CR1_CEN;
}

void Delay_Us(uint32_t us) {
    uint16_t start = TIM9->CNT;   // Valeur initiale
    uint32_t elapsed = 0;

    while (elapsed < us) {
        // Le calcul fonctionne même si le compteur déborde (modulo 65536)
        elapsed = (uint32_t)(TIM9->CNT - start);
    }
}

void Delay_Ms(uint32_t ms) {
    while (ms--) {
        Delay_Us(1000);   // 1 ms = 1000 µs
    }
}