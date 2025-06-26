/*
 * File:   newmain.c
 * Author: Beatriz S. Tartare e Maria Eduarda W. M. Vianna
 *
 * Created on 19 de Junho de 2025, 15:55
 */

// https://github.com/kwiecinski/PIC-XC8-KS0108-NT7108-LCD-LIBRARY/blob/master/Files/KS0108.h

// inclusao de bibliotecas
#include <xc.h>          //***inclus?o da biblioteca do compilador
#include <pic16f877a.h>  //***inclus?o da biblioteca do chip espec?fico
#include <stdio.h>       //***inclus?o da biblioteca standard padr?o "C"
#include "glcd.h"

#define _XTAL_FREQ 4000000 //***Define a frequencia do clock, 4Mhz neste caso

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = ON      // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit 


//Definindo bits de entrada e saída
#define BT_EM PORTBbits.RB0 // Interrupção, botao de emergencia
#define SN_X PORTBbits.RB1  // sensor particula x
#define SN_Y PORTBbits.RB2  // sensor particula y
#define LED_EM PORTBbits.RB3 // led emergencia
#define PRE_AC PORTBbits.RB4 // led para sinalizar que chegou no pre-acelerador
#define LHC PORTBbits.RB5 // led para sinalizar que chegou no LHC
#define COL PORTBbits.RB6 // led para sinalizar colisao

int conta = 0;
int energ = 0;
int temp = 0;
int rad = 0;
int flag = 0;

const unsigned char exemplo_imagem[1024] = {0};

int ler_an(int canal);
void mostrar_energia();
void mostrar_temperatura();
void mostrar_radiacao();
void mostrar_colisao();


void main(void) {
    OPTION_REG =0b00111111; // Ativa os pull-ups.
    TRISB = 0b00000111; // RB0, RB1, RB2 = entrada e RB3-RB7 = saída
    TRISA = 0b00000111;

    // Inicializa as saídas RB3 até RB6 em 0
    LED_EM = 0; 
    PRE_AC = 0;
    LHC = 0;
    COL = 0;
    
    //** configurando interrup??es ***********************************
    INTCONbits.GIE=1;        //Habiliita a int global
    INTCONbits.PEIE = 1;     //Habilita a int dos perif?ricos
    INTCONbits.INTE = 1;     // interrupção externa (RB0)
    PIE1bits.TMR1IE = 1;    //Habilita int do timer 1
    PIE1bits.ADIE = 1;      //Habilita a interrupção do ADC
    
    //*** configura o timer 1 *****************************************
    T1CONbits.TMR1CS = 0;   //Define timer 1 como temporizador (Fosc/4).
    // Obs.: sabendo que nosso PIC é 4MHz, Fosc/4 = 1MHz. Isso significa que, em 1s, contaria 1000000 pulsos.
    T1CONbits.T1CKPS0 = 1;  //bit pra configurar pre-escaler, neste caso 1:8
    T1CONbits.T1CKPS1 = 1;  //bit pra configurar pre-escaler, neste caso 1:8.
    // Obs.: agora, temos 1000000/8 = 125000 contagens por segundo. Ou seja, 1 contagem a cada 8 microssegundos.
        
    // Timer 1 é de 16 bits. Vai de 0 até 65535.
    TMR1L = 0xDC;          //carga do valor inicial no contador (65536-62500)
    TMR1H = 0x0B;          //3036. Quando estourar contou 62500, passou 0,5s   

    //*** Configura entradas analógicas *****************************************
    ADCON1bits.PCFG0   = 0;
    ADCON1bits.PCFG1   = 1;
    ADCON1bits.PCFG2   = 1;
    ADCON1bits.PCFG3   = 1;

    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;

    ADCON1bits.ADFM = 1;

    //inicializa registradores do AD
    ADRESL = 0x00;          //inicializar valor anal?gico com 0
    ADRESH = 0x00;          

    ADCON0bits.ADON = 1;     //Liga AD

    glcd_init();
    glcd_clear();
    
    //** configura a taxa de temporiza��o do WatchDog Time(WDT)1:2 neste caso 36ms
   //Desde que PSA = 1, ou seja, pre-escaler do timer0 exclusivo para WDT, pode usar
   //timer 0, mas conta 1:1
    OPTION_REGbits.PS0 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 1;
     
    CLRWDT();    //reseta a contagem do WDT para n�o resetar
    
    
    while(1){
    
        CLRWDT();    //reseta a contagem do WDT para n�o resetar
        
        //leitura da radiação para interrupcao
        if (ADCON0bits.GO == 0) rad = ler_an(1);
        
        // Se os sensores de feixes de prótons estiverem ativos, iniciamos o processo de aceleração.
        if (SN_X == 0 && SN_Y == 0 && flag==0)
        {
             //ativação do timer
            T1CONbits.TMR1ON = 1;
            flag = 1;
        }
         
        while(flag==1)
        {
            CLRWDT();
            //leitura da radiação para interrupcao
            if (ADCON0bits.GO == 0) rad = ler_an(1);
            
            //leitura da energia
            energ = ler_an(2);
            mostrar_energia();
            mostrar_temperatura();
            mostrar_radiacao();
           
            __delay_ms(50);
            
            //se maior que 600 entra no lhc
            if(energ>600) 
            {
               PRE_AC = 0;
               LHC = 1;

               //se maior que 800 colisao
                if(energ>800)
                {
                    COL = 1;
                   CLRWDT();
                   mostrar_colisao();
                   CLRWDT();
                   flag = 0;
                }
           }   
        }
        //desliga leds
        COL = 0;
        LHC = 0;
    }
    return;
    
    /* EXEMPLO DE ESCRITA NO DISPLAY!!!!!!!!
    // Pequeno atraso na inicialização para estabilização da fonte/PIC
    // Especialmente útil em simulações ou hardware real
    __delay_ms(100);

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
    if (TMR1IF)  //foi a interrupcao de estouro do timer1 que chamou a int?
     {  
        PIR1bits.TMR1IF = 0; //reseta o flag da interrupcao
        TMR1L = 0xDC;        //reinicia contagem com 3036
        TMR1H = 0x0B;        
        
        //comandos pra tratar a interrupcao
        conta++;
        //conta == 8 passou 5s
        if (conta==8){
            //flag_entrou_pre_ac = 1; // Ativando flag q mostra q particulas entraram no pre acelerador.
            PRE_AC = 1; 
            LHC = 0;
            COL = 0;
            conta = 0;
            T1CONbits.TMR1ON = 0; 
        }
    }
    if (INTF) // Foi a interrupção externa que chamou?
    {
        INTCONbits.INTF = 0; // Reseta flag de interrup??o.
        
        while (BT_EM == 0) // Enquanto botão de emergência pressionado
        {
            LED_EM = 1; // Liga led de emergência
            
            // Desligar todos os outros LEDs.
            PRE_AC = 0;
            LHC = 0;
            COL = 0;
        }
        LED_EM = 0; // Desligar led de emergência.
    }
    
    if (ADIF) //foi a interrupcaoo de final de conversao AD?
    {
        PIR1bits.ADIF = 0; //reseta o flag da interrupcao

        if (ADCON0bits.CHS == 1) //verifica se é o canal 1
        { 
            while(rad>800) //enquanto > 800: protocolo de emergencia
            {
                rad = ler_an(1); //le a entrada analogica de radiação 
                LED_EM = 1; // Liga led de emergência

                // Desliga todos os outros LEDs.
                PRE_AC = 0;
                LHC = 0;
                COL = 0;
            }
            LED_EM = 0; // Desliga led de emergência.
        }
    }
}
int ler_an(int canal) 
{
    ADCON0bits.CHS = canal;  // Seleciona o canal ADC (ex: 0 = AN0, 1 = AN1, etc.)
    ADCON0bits.GO = 1;   // Inicia a conversão ADC
     __delay_us(100);   
     
     // Junta os dois registradores de 8 bits (ADRESH e ADRESL)
    return ((ADRESH << 8) + ADRESL);
}
void mostrar_colisao()
{
    // 6 Posicoes
    const uint8_t posicoes[6][4] = {
        {60, 60, 67, 60},
        {35, 52, 105, 52},
        {10, 38, 117, 38},
        {10, 27, 117, 27},
        {35, 16, 105, 16},
        {60, 5, 67, 5}
    };

    for (int i = 0; i < 6; i++) {
        glcd_clear();

        // Esquerda
        uint8_t x1 = posicoes[i][0];
        uint8_t y1 = posicoes[i][1];
        glcd_draw_char('.', y1 / 8, x1);

        // Direita
        uint8_t x2 = posicoes[i][2];
        uint8_t y2 = posicoes[i][3];
        glcd_draw_char('.', y2 / 8, x2);

        // "COLISAO" no meio fixo
        char *msg = "COLISAO";
        uint8_t col = 64 - 3 * 6;
        for (int j = 0; msg[j] != '\0'; j++) {
            glcd_draw_char(msg[j], 4, col);  // página 4 ? y = 32
            col += 6;
        }

        __delay_ms(300);
    }
}
void mostrar_radiacao()
{
    int valor_rad = ler_an(1); // Lê o valor da radiacao

    char buffer[16]; // Buffer para armazenar a string da radiacao

    // Limpa a área onde a radiacao será exibida para evitar lixo de leituras anteriores
    glcd_write_string("               ", 5, 0); // Limpa a linha 5 inteira (15 espaços)

    // Converte o valor_energia para string.
    sprintf(buffer, "%d U", valor_rad); 
    
    // Exibe o texto "RADIACAO"
    glcd_write_string("RADIACAO", 4, 0); 
    
    // Exibe o valor da radiacao
    glcd_write_string(buffer, 5, 0); 

    // O delay aqui pode ser ajustado ou movido para a sua função principal (main)
    // se você quiser atualizar o display mais frequentemente ou ter controle externo.
    __delay_ms(1000); 
    
}
void mostrar_temperatura()
{
    int valor_temperatura = (ler_an(0)/2) - 1; // Lê o valor da temepratura (ver porque dimiui 1)

    char buffer[16]; // Buffer para armazenar a string da temperatura

    // Limpa a área onde a temperatura será exibida para evitar lixo de leituras anteriores
    glcd_write_string("               ", 3, -1); // Limpa a linha 3 inteira (15 espaços)

    // Converte o valor_energia para string.
    sprintf(buffer, "%d C", valor_temperatura); 
    
    // Exibe o texto "TEMPERATURA"
    glcd_write_string("TEMPERATURA", 2, -1); 
    
    // Exibe o valor da temperatura 
    glcd_write_string(buffer, 3, -1); //ver porque o -1 faz a temperatura nao cortar

    // O delay aqui pode ser ajustado ou movido para a sua função principal (main)
    // se você quiser atualizar o display mais frequentemente ou ter controle externo.
    __delay_ms(100); 
    
}
void mostrar_energia() {
    int valor_energia = ler_an(2); // Lê o valor da energia

    char buffer[16]; // Buffer para armazenar a string da energia (ex: "12345 kWh")

    // Limpa a área onde a energia será exibida para evitar lixo de leituras anteriores
    glcd_write_string("               ", 0, 0); // Limpa a linha 1 inteira (15 espaços)

    // Converte o valor_energia para string.
    sprintf(buffer, "%d kWh", valor_energia); 
    
    // Exibe o texto "ENERGIA:"
    glcd_write_string("ENERGIA:", 0, 0); 
    
    // Exibe o valor da energia na página 1 (abaixo de "ENERGIA:")
    glcd_write_string(buffer, 1, 0); 

    // O delay aqui pode ser ajustado ou movido para a sua função principal (main)
    // se você quiser atualizar o display mais frequentemente ou ter controle externo.
    __delay_ms(100); 
}
