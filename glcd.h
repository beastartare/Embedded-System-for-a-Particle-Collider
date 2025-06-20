/*
 * File:   glcd.h
 * Author: Usuario
 *
 * Created on 19 de Junho de 2025, 16:06
 */

#ifndef GLCD_H
#define GLCD_H

#include <xc.h>
#include <stdint.h>

// CONFIGURAÇÃO DOS PINOS
// Certifique-se de que estes pinos correspondem ao seu esquema no Proteus e PIC
#define GLCD_DATA_PORT      PORTD
#define GLCD_DATA_TRIS      TRISD

#define GLCD_RS             PORTCbits.RC4
#define GLCD_RW             PORTCbits.RC5
#define GLCD_EN             PORTCbits.RC6
#define GLCD_RST            PORTCbits.RC7 // Pino de Reset
#define GLCD_CS1            PORTCbits.RC2 // Chip Select 1 (Lado esquerdo do display)
#define GLCD_CS2            PORTCbits.RC3 // Chip Select 2 (Lado direito do display)

#define GLCD_RS_DIR         TRISCbits.TRISC4
#define GLCD_RW_DIR         TRISCbits.TRISC5
#define GLCD_EN_DIR         TRISCbits.TRISC6
#define GLCD_RST_DIR        TRISCbits.TRISC7
#define GLCD_CS1_DIR        TRISCbits.TRISC2
#define GLCD_CS2_DIR        TRISCbits.TRISC3

// Declarações das funções
void glcd_init(void);
void glcd_clear(void);
void glcd_set_page(uint8_t page);
void glcd_set_column(uint8_t column);
void glcd_write_data(uint8_t data);
void glcd_display_image(const uint8_t *image); // Para imagens bitmap
void glcd_draw_char(char c, uint8_t page, uint8_t column); // Para desenhar caracteres
void glcd_write_string(const char *str, uint8_t page, uint8_t start_column); // Adicionada para escrever strings

#endif
