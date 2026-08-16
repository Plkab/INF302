/*
 * Fichier : pt_ista_stm32f4_v1.h
 * Auteur : plkab Paul S. Kabidu
 * Paul S. Kabidu, ISTA Goma
 * Crée le 18 Aout 2026
 */



/*
 * Copyright (c) 2004-2005, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: pt.h,v 1.7 2006/10/02 07:52:56 adam Exp $
 */

/**
 * \addtogroup pt
 * @{
 */

/**
 * \file
 * Protothreads implementation.
 * \author
 * Adam Dunkels <adam@sics.se>
 *
 */

#ifndef __PT_H__
#define __PT_H__





///////////////////////////////////////////////////////
//#include "lc.h"
///////////////////////////////////////////////////////

/**
 * \file lc.h
 * Local continuations
 * \author
 * Adam Dunkels <adam@sics.se>
 *
 */

#ifdef DOXYGEN
/**
 * Initialize a local continuation.
 *
 * This operation initializes the local continuation, thereby
 * unsetting any previously set continuation state.
 *
 * \hideinitializer
 */
#define LC_INIT(lc)

/**
 * Set a local continuation.
 *
 * The set operation saves the state of the function at the point
 * where the operation is executed. As far as the set operation is
 * concerned, the state of the function does <b>not</b> include the
 * call-stack or local (automatic) variables, but only the program
 * counter and such CPU registers that needs to be saved.
 *
 * \hideinitializer
 */
#define LC_SET(lc)

/**
 * Resume a local continuation.
 *
 * The resume operation resumes a previously set local continuation, thus
 * restoring the state in which the function was when the local
 * continuation was set. If the local continuation has not been
 * previously set, the resume operation does nothing.
 *
 * \hideinitializer
 */
#define LC_RESUME(lc)

/**
 * Mark the end of local continuation usage.
 *
 * The end operation signifies that local continuations should not be
 * used any more in the function. This operation is not needed for
 * most implementations of local continuation, but is required by a
 * few implementations.
 *
 * \hideinitializer 
 */
#define LC_END(lc)

/**
 * \var typedef lc_t;
 *
 * The local continuation type.
 *
 * \hideinitializer
 */
#endif /* DOXYGEN */

//#ifndef __LC_H__
//#define __LC_H__


//#ifdef LC_INCLUDE
//#include LC_INCLUDE
//#else
//#include "lc-switch.h"
//#endif /* LC_INCLUDE */

//#endif /* __LC_H__ */

/** @} */
/** @} */












////////////////////////////////////////////////////////
//#include "lc-switch.h"
////////////////////////////////////////////////////////


//#ifndef __LC_SWITCH_H__
//#define __LC_SWITCH_H__

/* WARNING! lc implementation using switch() does not work if an
   LC_SET() is done within another switch() statement! */

/** \hideinitializer */
/*
typedef unsigned short lc_t;

#define LC_INIT(s) s = 0;

#define LC_RESUME(s) switch(s) { case 0:

#define LC_SET(s) s = __LINE__; case __LINE__:

#define LC_END(s) }

#endif /* __LC_SWITCH_H__ */

/** @} */




//#endif /* LC_INCLUDE */

//#endif /* __LC_H__ */

/** @} */
/** @} */

















////////////////////////////////////////////////////////
//#include "lc-addrlabels.h"
////////////////////////////////////////////////////////

#ifndef __LC_ADDRLABELS_H__
#define __LC_ADDRLABELS_H__

/** \hideinitializer */
typedef void * lc_t;

#define LC_INIT(s) s = NULL

#define LC_RESUME(s)				\
  do {						\
    if(s != NULL) {				\
      goto *s;					\
    }						\
  } while(0)

#define LC_CONCAT2(s1, s2) s1##s2
#define LC_CONCAT(s1, s2) LC_CONCAT2(s1, s2)

#define LC_SET(s)				\
  do {						\
    LC_CONCAT(LC_LABEL, __LINE__):   	        \
    (s) = &&LC_CONCAT(LC_LABEL, __LINE__);	\
  } while(0)

#define LC_END(s)

#endif /* __LC_ADDRLABELS_H__ */














//////////////////////////////////////////////////////
// pt
//////////////////////////////////////////////////////

struct pt {
  lc_t lc;
};

#define PT_WAITING 0
#define PT_YIELDED 1
#define PT_EXITED  2
#define PT_ENDED   3

/**
 * \name Initialization
 * @{
 */

/**
 * Initialize a protothread.
 *
 * Initializes a protothread. Initialization must be done prior to
 * starting to execute the protothread.
 *
 * \param pt A pointer to the protothread control structure.
 *
 * \sa PT_SPAWN()
 *
 * \hideinitializer
 */
#define PT_INIT(pt)   LC_INIT((pt)->lc)

/** @} */

/**
 * \name Declaration and definition
 * @{
 */

/**
 * Declaration of a protothread.
 *
 * This macro is used to declare a protothread. All protothreads must
 * be declared with this macro.
 *
 * \param name_args The name and arguments of the C function
 * implementing the protothread.
 *
 * \hideinitializer
 */
#define PT_THREAD(name_args) char name_args

/**
 * Declare the start of a protothread inside the C function
 * implementing the protothread.
 *
 * This macro is used to declare the starting point of a
 * protothread. It should be placed at the start of the function in
 * which the protothread runs. All C statements above the PT_BEGIN()
 * invokation will be executed each time the protothread is scheduled.
 *
 * \param pt A pointer to the protothread control structure.
 *
 * \hideinitializer
 */
#define PT_BEGIN(pt) { char PT_YIELD_FLAG = 1; LC_RESUME((pt)->lc)

/**
 * Declare the end of a protothread.
 *
 * This macro is used for declaring that a protothread ends. It must
 * always be used together with a matching PT_BEGIN() macro.
 *
 * \param pt A pointer to the protothread control structure.
 *
 * \hideinitializer
 */
#define PT_END(pt) LC_END((pt)->lc); PT_YIELD_FLAG = 0; \
                   PT_INIT(pt); return PT_ENDED; }

/** @} */

/**
 * \name Blocked wait
 * @{
 */

/**
 * Block and wait until condition is true.
 *
 * This macro blocks the protothread until the specified condition is
 * true.
 *
 * \param pt A pointer to the protothread control structure.
 * \param condition The condition.
 *
 * \hideinitializer
 */
#define PT_WAIT_UNTIL(pt, condition)	        \
  do {						\
    LC_SET((pt)->lc);				\
    if(!(condition)) {				\
      return PT_WAITING;			\
    }						\
  } while(0)

/**
 * Block and wait while condition is true.
 *
 * This function blocks and waits while condition is true. See
 * PT_WAIT_UNTIL().
 *
 * \param pt A pointer to the protothread control structure.
 * \param cond The condition.
 *
 * \hideinitializer
 */
#define PT_WAIT_WHILE(pt, cond)  PT_WAIT_UNTIL((pt), !(cond))

/** @} */

/**
 * \name Hierarchical protothreads
 * @{
 */

/**
 * Block and wait until a child protothread completes.
 *
 * This macro schedules a child protothread. The current protothread
 * will block until the child protothread completes.
 *
 * \note The child protothread must be manually initialized with the
 * PT_INIT() function before this function is used.
 *
 * \param pt A pointer to the protothread control structure.
 * \param thread The child protothread with arguments
 *
 * \sa PT_SPAWN()
 *
 * \hideinitializer
 */
#define PT_WAIT_THREAD(pt, thread) PT_WAIT_WHILE((pt), PT_SCHEDULE(thread))

/**
 * Spawn a child protothread and wait until it exits.
 *
 * This macro spawns a child protothread and waits until it exits. The
 * macro can only be used within a protothread.
 *
 * \param pt A pointer to the protothread control structure.
 * \param child A pointer to the child protothread's control structure.
 * \param thread The child protothread with arguments
 *
 * \hideinitializer
 */
#define PT_SPAWN(pt, child, thread)		\
  do {						\
    PT_INIT((child));				\
    PT_WAIT_THREAD((pt), (thread));		\
  } while(0)

/** @} */

/**
 * \name Exiting and restarting
 * @{
 */

/**
 * Restart the protothread.
 *
 * This macro will block and cause the running protothread to restart
 * its execution at the place of the PT_BEGIN() call.
 *
 * \param pt A pointer to the protothread control structure.
 *
 * \hideinitializer
 */
#define PT_RESTART(pt)				\
  do {						\
    PT_INIT(pt);				\
    return PT_WAITING;			\
  } while(0)

/**
 * Exit the protothread.
 *
 * This macro causes the protothread to exit. If the protothread was
 * spawned by another protothread, the parent protothread will become
 * unblocked and can continue to run.
 *
 * \param pt A pointer to the protothread control structure.
 *
 * \hideinitializer
 */
#define PT_EXIT(pt)				\
  do {						\
    PT_INIT(pt);				\
    return PT_EXITED;			\
  } while(0)

/** @} */

/**
 * \name Calling a protothread
 * @{
 */

/**
 * Schedule a protothread.
 *
 * This function shedules a protothread. The return value of the
 * function is non-zero if the protothread is running or zero if the
 * protothread has exited.
 *
 * \param f The call to the C function implementing the protothread to
 * be scheduled
 *
 * \hideinitializer
 */
#define PT_SCHEDULE(f) ((f) < PT_EXITED)

/** @} */

/**
 * \name Yielding from a protothread
 * @{
 */

/**
 * Yield from the current protothread.
 *
 * This function will yield the protothread, thereby allowing other
 * processing to take place in the system.
 *
 * \param pt A pointer to the protothread control structure.
 *
 * \hideinitializer
 */
#define PT_YIELD(pt)				\
  do {						\
    PT_YIELD_FLAG = 0;				\
    LC_SET((pt)->lc);				\
    if(PT_YIELD_FLAG == 0) {			\
      return PT_YIELDED;			\
    }						\
  } while(0)

/**
 * \brief      Yield from the protothread until a condition occurs.
 * \param pt   A pointer to the protothread control structure.
 * \param cond The condition.
 *
 *             This function will yield the protothread, until the
 *             specified condition evaluates to true.
 *
 *
 * \hideinitializer
 */
#define PT_YIELD_UNTIL(pt, cond)		\
  do {						\
    PT_YIELD_FLAG = 0;				\
    LC_SET((pt)->lc);				\
    if((PT_YIELD_FLAG == 0) || !(cond)) {	\
      return PT_YIELDED;			\
    }						\
  } while(0)

/** @} */

#endif /* __PT_H__ */

/** @} */












/////////////////////////////////////////////////////////
// SEMAPHORES
/////////////////////////////////////////////////////////



#endif /* __PT_H__ */

#ifndef __PT_SEM_H__
#define __PT_SEM_H__

//#include "pt.h"

struct pt_sem {
  unsigned int count;
};

/**
 * Initialize a semaphore
 *
 * This macro initializes a semaphore with a value for the
 * counter. Internally, the semaphores use an "unsigned int" to
 * represent the counter, and therefore the "count" argument should be
 * within range of an unsigned int.
 *
 * \param s (struct pt_sem *) A pointer to the pt_sem struct
 * representing the semaphore
 *
 * \param c (unsigned int) The initial count of the semaphore.
 * \hide initializer
 */
// NOTE that the default semaphore is not
// multi-core safe, but is OK one one core

#define PT_SEM_INIT(s, c) (s)->count = c

/**
 * Wait for a semaphore
 *
 * This macro carries out the "wait" operation on the semaphore. The
 * wait operation causes the protothread to block while the counter is
 * zero. When the counter reaches a value larger than zero, the
 * protothread will continue.
 *
 * \param pt (struct pt *) A pointer to the protothread (struct pt) in
 * which the operation is executed.
 *
 * \param s (struct pt_sem *) A pointer to the pt_sem struct
 * representing the semaphore
 *
 * \hideinitializer
 */
#define PT_SEM_WAIT(pt, s)	\
  do {						\
    PT_YIELD_UNTIL(pt, (s)->count > 0);		\
    --(s)->count;				\
  } while(0)

/**
 * Signal a semaphore
 *
 * This macro carries out the "signal" operation on the semaphore. The
 * signal operation increments the counter inside the semaphore, which
 * eventually will cause waiting protothreads to continue executing.
 *
 * \param pt (struct pt *) A pointer to the protothread (struct pt) in
 * which the operation is executed.
 *
 * \param s (struct pt_sem *) A pointer to the pt_sem struct
 * representing the semaphore
 *
 * \hideinitializer
 */
//#define PT_SEM_SIGNAL(pt, s) ++(s)->count
#define PT_SEM_SIGNAL(pt,s) ++(s)->count

#endif /* __PT_SEM_H__ */













//////////////////////////////////////////////////////
// Ajout par Paul S. Kabidu
//////////////////////////////////////////////////////



/////////////////////////////////////////////////////
// TIMERS
////////////////////////////////////////////////////

#include "stm32f4xx.h"
#include <stdint.h>

// Variable globale comptant les millisecondes (Autonomie : ~49 jours)
static volatile uint32_t system_ms = 0;

/**
 * @brief Initialise le SysTick pour déborder toutes les 1 ms
 * Horloge HSI = 16 MHz -> 16 000 000 / 1000 = 16 000 cycles pour 1 ms
 */
void SysTick_Time_Init(void) {
    SysTick->LOAD = 16000 - 1; 
    SysTick->VAL  = 0;         // Force la réinitialisation initiale
    
    // Configuration : Source interne + Interruption activée + Périphérique actif
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
                    SysTick_CTRL_TICKINT_Msk   | 
                    SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief Interruption du SysTick (Exécutée de manière matérielle toutes les 1 ms)
 */
void SysTick_Handler(void) {
    system_ms++; 
}

/**
 * @brief Retourne le temps système actuel en microsecondes (32 bits)
 * Déborde et revient à zéro naturellement toutes les 71,5 minutes (2^32 µs).
 */
uint32_t time_us_32(void) {
    uint32_t ms, ticks, ms_verify;

    // Boucle de lecture sécurisée anti-débordement
    do {
        ms = system_ms;
        ticks = SysTick->VAL; // Registre descendant (15999 -> 0)
        ms_verify = system_ms;
    } while (ms != ms_verify);

    // Calcul des ticks de processeur consommés dans la milliseconde en cours
    uint32_t elapsed_ticks = 16000 - ticks;
    uint32_t partial_us = elapsed_ticks / 16; // 16 ticks = 1 µs à 16 MHz

    return (ms * 1000) + partial_us;
}









// Déclaration externe de l'horloge système
extern uint32_t time_us_32(void);

// Variable de contrôle requise pour valider l'exécution de l'ordonnanceur coopératif
extern volatile int pt_executed;

// 1. Macro d'attente relative (Invisible, sans structure manuelle externe)
#define PT_YIELD_usec(delay_time) \
    do { \
        static uint32_t pt_time_target; \
        pt_time_target = time_us_32() + (uint32_t)(delay_time); \
        PT_YIELD_UNTIL(pt, ((int32_t)(time_us_32() - pt_time_target) >= 0)); \
    } while(0)


/*
 * Initialise le marqueur pour un cadencement à intervalle strict.
 * Doit impérativement être placée au début du protothread, avant la macro PT_BEGIN.
 */
// 2. Macros de cadencement strict (Anti-Dérive pour boucles infinies)
#define PT_INTERVAL_INIT() static uint32_t pt_interval_marker

/*
 * Force la tâche à s'exécuter à un rythme périodique fixe et absolu (Anti-Dérive)
 * Réajuste dynamiquement le temps d'attente en soustrayant le temps pris par les calculs.
 */
#define PT_YIELD_INTERVAL(interval_time) \
    do { \
        PT_YIELD_UNTIL(pt, ((int32_t)(time_us_32() - pt_interval_marker) >= 0)); \
        pt_interval_marker = time_us_32() + (uint32_t)(interval_time); \
        pt_executed = 1; \
    } while(0)

#endif // PT_TIME_H











/////////////////////////////////////////////////////
// MUTEX (Verrouillage pour sections critiques)
/////////////////////////////////////////////////////

/**
 * \brief Structure représentant un mutex binaire.
 * 
 *  locked = 0 → libre
 *  locked = 1 → verrouillé
 */
typedef struct {
    volatile uint8_t locked;
} pt_mutex_t;

/**
 * \brief Initialiser un mutex (à l'état libre).
 * \param m  Pointeur vers le mutex.
 */
#define PT_MUTEX_INIT(m)   ((m)->locked = 0)

/**
 * \brief Verrouiller le mutex de manière non‑bloquante.
 * 
 * Si le mutex est libre, il est verrouillé et la tâche continue.
 * Sinon, la tâche est suspendue (rend la main) jusqu'à ce qu'il soit libre.
 * 
 * \param pt  Pointeur vers le protothread en cours.
 * \param m   Pointeur vers le mutex.
 */
#define PT_MUTEX_LOCK(pt, m) \
    do { \
        PT_WAIT_UNTIL(pt, (m)->locked == 0); \
        (m)->locked = 1; \
    } while(0)

/**
 * \brief Déverrouiller le mutex.
 * \param m  Pointeur vers le mutex.
 */
#define PT_MUTEX_UNLOCK(m) \
    do { \
        (m)->locked = 0; \
    } while(0)

/////////////////////////////////////////////////////