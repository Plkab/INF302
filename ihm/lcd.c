/**
 * @file lcd.c
 * @brief Implémentation du driver LCD HD44780 en mode 4 bits sur PORT B.
 * 
 * Toutes les fonctions sont bloquantes (utilisent Delay_Us / Delay_Ms).
 * Pour une utilisation non-bloquante, il faudrait une machine à états.
 * Ici, l'accent est mis sur la clarté et la modularité.
 */

#include "lcd.h"

/* ============================================================
 * 1. TABLEAU DES OFFSETS DDRAM POUR LES 2 LIGNES
 *    (Adresses de début de ligne dans la mémoire du LCD)
 * ============================================================ */
static const uint8_t ROW_OFFSETS[] = {0x00, 0x40};  // Ligne 0 -> 0x00, Ligne 1 -> 0x40

/* ============================================================
 * 2. FONCTIONS PRIVÉES (statiques)
 *    Elles ne sont pas accessibles depuis l'extérieur du fichier.
 * ============================================================ */

/**
 * @brief Génère une impulsion sur la broche EN (Enable) du LCD.
 * 
 * La séquence est : EN = HIGH pendant 10 µs, puis LOW pendant 100 µs.
 * C'est la base de la communication série (4 bits) avec le HD44780.
 */
static void LCD_Enable(void) {
    LCD_PORT->ODR |= EN_MASK;      // Met la broche EN à HIGH
    Delay_Us(1);                  // Maintient 10 µs (temps de setup)
    LCD_PORT->ODR &= ~EN_MASK;     // Met la broche EN à LOW
    Delay_Us(80);                 // Temps de stabilisation (setup)
}

/**
 * @brief Envoie un quartet (4 bits) sur les broches D4-D7.
 * @param data  Les 4 bits à envoyer (les 4 bits de poids faible sont utilisés).
 * 
 * On efface d'abord les 4 broches de données, puis on applique la nouvelle valeur.
 * Ensuite on génère une impulsion sur EN pour valider la transmission.
 */
static void LCD_Send4Bits(uint8_t data) {
    // Efface les 4 bits de données (les met à 0)
    LCD_PORT->ODR &= ~DATA_MASK;

    // Place les 4 bits de data sur les broches D4-D7
    // (On décale de D4_PIN car D4 est le bit de poids faible du quartet)
    LCD_PORT->ODR |= ((data & 0x0F) << D4_PIN);
    
    	// Petit délai pour laisser les données se stabiliser (tSU)
    Delay_Us(100);
    /////////////////////////////////////////////////////////////

    // Valide par une impulsion sur EN
    LCD_Enable();
}

/**
 * @brief Envoie une commande complète (8 bits) au LCD.
 * @param cmd  La commande à envoyer (8 bits).
 * 
 * La commande est envoyée en deux fois : d'abord le nibble haut, puis le nibble bas.
 * Le signal RS est mis à 0 (mode commande).
 * Pour les commandes CLEAR et HOME, on attend 2ms (temps nécessaire au LCD).
 */
static void LCD_SendCommand(uint8_t cmd) {
    LCD_PORT->ODR &= ~RS_MASK;    // RS = 0 (sélectionne le registre de commande)

    LCD_Send4Bits(cmd >> 4);      // Envoie les 4 bits de poids fort (nibble haut)
    LCD_Send4Bits(cmd & 0x0F);    // Envoie les 4 bits de poids faible (nibble bas)

    // Ces deux commandes nécessitent un temps de traitement plus long
    if (cmd == LCD_CLEAR || cmd == LCD_HOME) {
        Delay_Ms(2);              // Attente 2 ms (spécification HD44780)
    }
}

/**
 * @brief Envoie une donnée (un caractère) à afficher.
 * @param data  Le code ASCII ou le caractère à afficher.
 * 
 * Similaire à l'envoi d'une commande, mais RS = 1 (mode données).
 * Un délai de 100 µs est ajouté pour la stabilisation.
 */
static void LCD_SendData(uint8_t data) {
    LCD_PORT->ODR |= RS_MASK;     // RS = 1 (sélectionne le registre de données)

    LCD_Send4Bits(data >> 4);     // Nibble haut
    LCD_Send4Bits(data & 0x0F);   // Nibble bas

    //Delay_Us(1);                // Délai de stabilisation (datasheet)
}

/* ============================================================
 * 3. FONCTION D'INITIALISATION (publique)
 * ============================================================ */

/**
 * @brief Initialise le LCD en mode 4 bits, 2 lignes, affichage ON, curseur OFF.
 * 
 * Cette fonction doit être appelée une fois au démarrage (avec Delay_Init déjà faite).
 * Elle suit la séquence d'initialisation recommandée par le fabricant.
 * 
 * Étapes :
 *   - Activer l'horloge du PORT B.
 *   - Configurer les broches RS, EN, D4-D7 en sortie (mode 0b01).
 *   - Mettre toutes les broches à HIGH (état initial).
 *   - Séquence spéciale d'initialisation (3x 0x03, 0x02).
 *   - Configurer le mode (4 bits, 2 lignes, 5x8 caractères).
 *   - Allumer l'affichage, éteindre le curseur.
 *   - Définir le mode d'entrée (incrément, décalage).
 *   - Effacer l'écran.
 */
void LCD_Init(void) {
    // --- 1. Activer l'horloge du PORT B ---
    RCC->AHB1ENR |= LCD_PORT_RCC;   // Active l'horloge pour le PORT B

    // --- 2. Configurer chaque broche en mode sortie (0b01 dans MODER) ---
    // Pour chaque broche, on efface les 2 bits de MODER, puis on met le bit 0 à 1.
    // RS_PIN
    LCD_PORT->MODER &= ~(3 << (RS_PIN * 2));
    LCD_PORT->MODER |=  (1 << (RS_PIN * 2));

    // EN_PIN
    LCD_PORT->MODER &= ~(3 << (EN_PIN * 2));
    LCD_PORT->MODER |=  (1 << (EN_PIN * 2));

    // D4_PIN
    LCD_PORT->MODER &= ~(3 << (D4_PIN * 2));
    LCD_PORT->MODER |=  (1 << (D4_PIN * 2));

    // D5_PIN
    LCD_PORT->MODER &= ~(3 << (D5_PIN * 2));
    LCD_PORT->MODER |=  (1 << (D5_PIN * 2));

    // D6_PIN
    LCD_PORT->MODER &= ~(3 << (D6_PIN * 2));
    LCD_PORT->MODER |=  (1 << (D6_PIN * 2));

    // D7_PIN
    LCD_PORT->MODER &= ~(3 << (D7_PIN * 2));
    LCD_PORT->MODER |=  (1 << (D7_PIN * 2));

    // --- 3. État initial des broches (toutes à HIGH par sécurité) ---
    LCD_PORT->BSRR = EN_MASK | D4_MASK | D5_MASK | D6_MASK | D7_MASK;

    // --- 4. Séquence d'initialisation spécifique au HD44780 ---
    // Attendre 50 ms après la mise sous tension
    Delay_Ms(50);

    // Envoyer trois fois 0x03 pour forcer le mode 8 bits (même si on ne le sait pas encore)
    LCD_Send4Bits(0x03); Delay_Ms(5);
    LCD_Send4Bits(0x03); Delay_Us(150);
    LCD_Send4Bits(0x03); Delay_Us(150);

    // Passage en mode 4 bits (commande 0x02)
    LCD_Send4Bits(0x02); Delay_Us(150);

    // --- 5. Configuration finale ---
    LCD_SendCommand(0x28);  // Function Set : 4 bits, 2 lignes, 5x8 caractères
    LCD_SendCommand(0x0C);  // Display ON, Cursor OFF, Blink OFF
    LCD_SendCommand(0x06);  // Entry mode : incrément, pas de décalage
    LCD_SendCommand(0x01);  // Clear display (efface tout)
    Delay_Ms(2);            // Attente de la fin de l'effacement
}

/* ============================================================
 * 4. FONCTIONS PUBLIQUES (API)
 *    Elles utilisent les fonctions privées ci-dessus.
 * ============================================================ */

void LCD_Clear(void) {
    LCD_SendCommand(LCD_CLEAR);   // Commande d'effacement
    Delay_Ms(2);                  // Temps nécessaire
}

void LCD_Home(void) {
    LCD_SendCommand(LCD_HOME);    // Retour à la case (0,0)
    Delay_Ms(2);
}

/**
 * @brief Affiche une chaîne de caractères à la position courante du curseur.
 */
void LCD_Print(char *str) {
    while (*str) {
        LCD_SendData(*str++);     // Envoie chaque caractère un par un
    }
}

/**
 * @brief Affiche une chaîne à une position donnée, en limitant à 16 caractères.
 * @param col  Colonne (0..15)
 * @param row  Ligne (0 ou 1)
 * @param text Chaîne à afficher (peut être plus longue, on tronque à 16).
 * 
 * La fonction positionne le curseur puis envoie les caractères jusqu'à
 * la fin de la ligne ou la fin de la chaîne.
 */
void LCD_PrintText(int col, int row, char *text) {
    // Vérification des bornes
    row = (row > 1) ? 1 : row;
    col = (col > 15) ? 15 : col;

    // Positionnement du curseur : on ajoute l'offset de la ligne
    LCD_SendCommand(LCD_SETDDRAMADDR | (col + ROW_OFFSETS[row]));

    // Envoi des caractères, en s'arrêtant si on dépasse 16 colonnes
    while (*text && (col++ < 16)) {
        LCD_SendData(*text++);
    }
}

void LCD_CursorOn(void) {
    LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON);
    Delay_Ms(1);
}

void LCD_CursorOff(void) {
    LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF);
    Delay_Ms(1);
}

void LCD_BlinkOn(void) {
    LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON);
    Delay_Ms(1);
}

void LCD_BlinkOff(void) {
    LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF);
    Delay_Ms(1);
}

void LCD_MoveCursor(int col, int row) {
    row = (row > 1) ? 1 : row;
    col = (col > 15) ? 15 : col;
    LCD_SendCommand(LCD_SETDDRAMADDR | (col + ROW_OFFSETS[row]));
    Delay_Ms(1);
}

void LCD_CursorShiftLeft(void) {
    LCD_SendCommand(LCD_CURSORSHIFT | 0x00);  // Décalage curseur gauche
    Delay_Ms(1);
}

void LCD_CursorShiftRight(void) {
    LCD_SendCommand(LCD_CURSORSHIFT | 0x04);  // Décalage curseur droite
    Delay_Ms(1);
}

void LCD_ShiftDisplayLeft(void) {
    LCD_SendCommand(LCD_CURSORSHIFT | 0x08);  // Décalage affichage gauche
    Delay_Ms(1);
}

void LCD_ShiftDisplayRight(void) {
    LCD_SendCommand(LCD_CURSORSHIFT | 0x0C);  // Décalage affichage droite
    Delay_Ms(1);
}

/**
 * @brief Crée un caractère personnalisé dans la CGRAM.
 * @param location  Emplacement (0 à 7) où stocker le motif.
 * @param charmap   Tableau de 8 octets définissant le motif (5x8 pixels).
 * 
 * Chaque octet représente une ligne de 8 pixels (seuls les 5 bits de poids faible
 * sont utilisés pour les caractères 5x8). Une fois créé, le caractère peut être
 * affiché avec LCD_WriteCustomChar(location).
 */
void LCD_CreateChar(unsigned char location, unsigned char charmap[]) {
    location &= 0x07;  // Limité à 0..7
    LCD_SendCommand(LCD_SETCGRAMADDR | (location << 3)); // Adresse de base du caractère
    for (int i = 0; i < 8; i++) {
        LCD_SendData(charmap[i]);   // Envoie les 8 lignes du motif
    }
    Delay_Ms(1);
}

/**
 * @brief Affiche un caractère personnalisé précédemment créé.
 * @param location  Emplacement (0..7) du caractère dans la CGRAM.
 */
void LCD_WriteCustomChar(unsigned char location) {
    location &= 0x07;
    LCD_SendData(location);   // Le code ASCII 0..7 correspond aux caractères CGRAM
    Delay_Ms(1);
}