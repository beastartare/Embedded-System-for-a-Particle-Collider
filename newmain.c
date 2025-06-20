/*
 * File:   newmain.c
 * Author: Usuario
 *
 * Created on 19 de Junho de 2025, 15:55
 */

// https://github.com/kwiecinski/PIC-XC8-KS0108-NT7108-LCD-LIBRARY/blob/master/Files/KS0108.h

#include <xc.h>          //***inclus?o da biblioteca do compilador
#include <pic16f877a.h>  //***inclus?o da biblioteca do chip espec?fico
#include <stdio.h>       //***inclus?o da biblioteca standard padr?o "C"
#include "glcd.h"

#define _XTAL_FREQ 4000000 //***Define a frequencia do clock, 4Mhz neste caso

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit 

const unsigned char exemplo_imagem[1024] = {0};
/*COMENTARIOS:
 "S1" no pic NÃO está conectada. Só está próxima.
 * 
 */

void main(void) {
    OPTION_REG =0b00111111; // Ativa os pull-ups.
    TRISB = 0b00000111; // RB0, RB1, RB2 = entrada e RB3-RB7 = saída
    INTCON = 0b10010000; // Ativando a interrupção externa.

    // Inicializa as saídas RB3 até RB6 em 0
    PORTBbits.RB3 = 0; 
    PORTBbits.RB4 = 0;
    PORTBbits.RB5 = 0;
    PORTBbits.RB6 = 0;
    
    while(1){
        // Se os sensores de feixes de prótons estiverem ativos, iniciamos o processo de aceleração.
        if (PORTBbits.RB1 == 0 && PORTBbits.RB2 == 0)
        {
            // Acende LED dos pré-aceleradores
            PORTBbits.RB4 = 1;
        }
        
    }
    return;
    
    /* EXEMPLO DE ESCRITA NO DISPLAY!!!!!!!!
    // Pequeno atraso na inicialização para estabilização da fonte/PIC
    // Especialmente útil em simulações ou hardware real
    __delay_ms(100);

    // Inicializa o GLCD
    glcd_init();

    // Limpa o display
    glcd_clear();

    // Atraso para ver o clear (opcional)
    __delay_ms(500);

    // Teste de escrita de caracteres individuais (agora pode ser qualquer um)
    glcd_draw_char('A', 0, 0);
    glcd_draw_char('B', 0, 6);
    glcd_draw_char('C', 0, 12);
    glcd_draw_char('1', 0, 18);
    glcd_draw_char('!', 0, 24);

    glcd_draw_char('O', 0, 64);  // 'O' na página 0, coluna 64 (Chip 2)
    glcd_draw_char('K', 0, 70);  // 'K' na página 0, coluna 70 (Chip 2)

    __delay_ms(2000); // Atraso para ver os caracteres

    // Teste de escrita de string
    glcd_clear(); // Limpa novamente
    __delay_ms(2000);

    glcd_write_string("OLA MUNDO", 0, 0); // Escreve na página 0, coluna 0 (Chip 1)
    glcd_write_string("TESTE GLCD", 1, 0); // Escreve na página 1, coluna 0 (Chip 1/2)
    glcd_write_string("123 ABC !@#$", 2, 0); // Exemplo com números e símbolos
    __delay_ms(4000); */
}

void __interrupt() TrataInt(void)
{
    if (INTF) // Foi a interrupção externa que chamou?
    {
        INTCONbits.INTF = 0; // Reseta flag de interrup??o.
        
        while (PORTBbits.RB0 == 0) // Enquanto botão de emergência pressionado
        {
            PORTBbits.RB3 = 1; // Liga botão de emergência
            
            // Desligar todos os outros LEDs.
            PORTBbits.RB4 = 0;
            PORTBbits.RB5 = 0;
            PORTBbits.RB6 = 0;
        }
        PORTBbits.RB3 = 0; // Desligar botão de emergência.
    }
}

