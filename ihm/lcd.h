/**
 * @file lcd.h
 * @brief Interface du driver LCD HD44780 en mode 4 bits sur PORT B.
 * 
 * Ce fichier définit les broches utilisées, les commandes du LCD et
 * les fonctions publiques pour piloter l'affichage.
 */

#ifndef LCD_H
#define LCD_H

#include "stm32f4xx.h"
#include "delay.h"   // Pour Delay_Us() et Delay_Ms()

/* ============================================================
 * 1. CHOIX DU PORT (modularité)
 *    Pour changer de port, il suffit de modifier LCD_PORT et
 *    LCD_PORT_RCC. Les macros de broches restent inchangées.
 * ============================================================ */
#define LCD_PORT        GPIOB           /**< Port utilisé (GPIOB) */
#define LCD_PORT_RCC    RCC_AHB1ENR_GPIOBEN  /**< Bit d'activation dans RCC */

/* ============================================================
 * 2. NUMÉROS DES BROCHES (physiques)
 *    Ces broches sont définies sur le PORT B de la Blackpill.
 * ============================================================ */
#define RS_PIN  9   /**< PB9  -> RS (Register Select) */
#define EN_PIN  10  /**< PB10 -> EN (Enable pulse)   */
#define D4_PIN  12  /**< PB12 -> D4 (Data bit 4)     */
#define D5_PIN  13  /**< PB13 -> D5 (Data bit 5)     */
#define D6_PIN  14  /**< PB14 -> D6 (Data bit 6)     */
#define D7_PIN  15  /**< PB15 -> D7 (Data bit 7)     */

/* ============================================================
 * 3. MASQUES BINAIRES (pour ODR, BSRR, etc.)
 *    Ces masques servent à manipuler les broches individuellement.
 * ============================================================ */
#define RS_MASK   (1 << RS_PIN)   /**< Masque pour RS  */
#define EN_MASK   (1 << EN_PIN)   /**< Masque pour EN  */
#define D4_MASK   (1 << D4_PIN)   /**< Masque pour D4  */
#define D5_MASK   (1 << D5_PIN)   /**< Masque pour D5  */
#define D6_MASK   (1 << D6_PIN)   /**< Masque pour D6  */
#define D7_MASK   (1 << D7_PIN)   /**< Masque pour D7  */
#define DATA_MASK (D4_MASK | D5_MASK | D6_MASK | D7_MASK) /**< Tous les bits de données */

/* ============================================================
 * 4. COMMANDES DU CONTROLEUR HD44780
 *    Ces constantes sont utilisées pour former les commandes.
 * ============================================================ */
#define LCD_CLEAR           0x01  /**< Efface l'écran et place le curseur en (0,0) */
#define LCD_HOME            0x02  /**< Retourne le curseur en (0,0) sans effacer   */
#define LCD_ENTRYMODE       0x04  /**< Définit le mode d'entrée (incrément/décrément) */
#define LCD_DISPLAYCONTROL  0x08  /**< Contrôle l'affichage, curseur, clignotement */
#define LCD_CURSORSHIFT     0x10  /**< Déplace le curseur ou l'écran               */
#define LCD_FUNCTIONSET     0x20  /**< Définit le nombre de lignes, taille des caractères */
#define LCD_SETCGRAMADDR    0x40  /**< Adresse en mémoire CGRAM (caractères personnalisés) */
#define LCD_SETDDRAMADDR    0x80  /**< Adresse en mémoire DDRAM (position du curseur) */

/* ============================================================
 * 5. SOUS-COMMANDES (combinaisons pour LCD_DISPLAYCONTROL)
 * ============================================================ */
#define LCD_DISPLAYON   0x04  /**< Allume l'affichage */
#define LCD_CURSORON    0x02  /**< Affiche le curseur (souligné) */
#define LCD_BLINKON     0x01  /**< Fait clignoter le curseur     */
#define LCD_CURSOROFF   0x00  /**< Cache le curseur              */
#define LCD_BLINKOFF    0x00  /**< Désactive le clignotement     */

/* ============================================================
 * 6. FONCTIONS PUBLIQUES
 *    Elles permettent d'utiliser le LCD de manière simple.
 * ============================================================ */

/** @brief Initialise le LCD (séquence de reset, configuration 4 bits, 2 lignes) */
void LCD_Init(void);

/** @brief Efface tout l'écran et ramène le curseur en (0,0) (bloquant 2ms) */
void LCD_Clear(void);

/** @brief Ramène le curseur en (0,0) sans effacer (bloquant 2ms) */
void LCD_Home(void);

/** @brief Affiche une chaîne de caractères à la position courante */
void LCD_Print(char *str);

/** @brief Affiche une chaîne à une position donnée (colonne, ligne) */
void LCD_PrintText(int col, int row, char *text);

/** @brief Affiche le curseur (souligné) */
void LCD_CursorOn(void);

/** @brief Cache le curseur */
void LCD_CursorOff(void);

/** @brief Active le clignotement du curseur */
void LCD_BlinkOn(void);

/** @brief Désactive le clignotement du curseur */
void LCD_BlinkOff(void);

/** @brief Déplace le curseur à la position (colonne, ligne) */
void LCD_MoveCursor(int col, int row);

/** @brief Déplace le curseur d'un cran vers la gauche */
void LCD_CursorShiftLeft(void);

/** @brief Déplace le curseur d'un cran vers la droite */
void LCD_CursorShiftRight(void);

/** @brief Décale tout l'affichage d'un cran vers la gauche */
void LCD_ShiftDisplayLeft(void);

/** @brief Décale tout l'affichage d'un cran vers la droite */
void LCD_ShiftDisplayRight(void);

/** @brief Crée un caractère personnalisé (8 octets) à l'emplacement 0..7 */
void LCD_CreateChar(unsigned char location, unsigned char charmap[]);

/** @brief Affiche un caractère personnalisé déjà créé (0..7) */
void LCD_WriteCustomChar(unsigned char location);

#endif /* LCD_H */