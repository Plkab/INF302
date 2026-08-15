/**
 * @file delay.h
 * @brief Interface des fonctions de délai utilisant le TIMER 9.
 * 
 * Ces fonctions sont **bloquantes** (le processeur boucle pendant l'attente).
 * Elles sont destinées aux initialisations et aux séquences temporelles critiques.
 * Pour un usage non-bloquant dans une boucle principale, utilisez des compteurs
 * basés sur un tick 1ms (ex: sys_tick).
 */

#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

/**
 * @brief Initialise le Timer 8 pour des délais en microsecondes.
 * 
 * À appeler une seule fois au démarrage, avant toute fonction Delay_Us() ou Delay_Ms().
 * Configure TIM9 avec un prescaler de 84-1 = 83 pour obtenir une fréquence de 1 MHz
 * (soit un tick toutes les 1 µs). Le compteur est mis sur 16 bits (ARR = 0xFFFF).
 */
void Delay_Init(void);

/**
 * @brief Bloque le programme pendant un nombre donné de microsecondes.
 * @param us  Durée en microsecondes (0 à 65535 µs maximum en une fois, mais
 *            on peut appeler plusieurs fois).
 * 
 * La fonction lit la valeur courante du compteur TIM9 puis attend que la
 * différence atteigne la valeur demandée. La gestion du débordement 16 bits
 * est automatique grâce à l'arithmétique non signée.
 */
void Delay_Us(uint32_t us);

/**
 * @brief Bloque le programme pendant un nombre donné de millisecondes.
 * @param ms  Durée en millisecondes (max 4294967295 ms, soit ~49 jours).
 * 
 * Cette fonction appelle simplement Delay_Us(1000) en boucle.
 * Pour des durées longues (> quelques secondes), une approche non-bloquante
 * serait préférable.
 */
void Delay_Ms(uint32_t ms);

#endif /* DELAY_H */