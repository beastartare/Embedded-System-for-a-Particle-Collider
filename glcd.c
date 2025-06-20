#include <xc.h>
#include "glcd.h"

#define _XTAL_FREQ 4000000 // Exemplo para 4MHz. Altere se sua frequência for diferente!

// Font 5x7 para caracteres ASCII imprimíveis (de ' ' a '~')
// Total de 95 caracteres. Cada caractere tem 5 bytes de dados.
const uint8_t font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 32: Space
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // 33: !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // 34: "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // 35: #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // 36: $
    {0x62, 0x64, 0x08, 0x13, 0x23}, // 37: %
    {0x36, 0x49, 0x56, 0x20, 0x50}, // 38: &
    {0x00, 0x08, 0x07, 0x00, 0x00}, // 39: '
    {0x00, 0x00, 0x3E, 0x41, 0x00}, // 40: (
    {0x00, 0x41, 0x3E, 0x00, 0x00}, // 41: )
    {0x0A, 0x0A, 0x7C, 0x0A, 0x0A}, // 42: *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // 43: +
    {0x00, 0x00, 0x50, 0x30, 0x00}, // 44: ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 45: -
    {0x00, 0x00, 0x60, 0x00, 0x00}, // 46: .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // 47: /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 48: 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 49: 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 50: 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 51: 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 52: 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 53: 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 54: 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 55: 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 56: 8
    {0x26, 0x49, 0x49, 0x4A, 0x3C}, // 57: 9
    {0x00, 0x00, 0x14, 0x00, 0x00}, // 58: :
    {0x00, 0x00, 0x14, 0x0C, 0x00}, // 59: ;
    {0x00, 0x08, 0x14, 0x22, 0x41}, // 60: <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // 61: =
    {0x41, 0x22, 0x14, 0x08, 0x00}, // 62: >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // 63: ?
    {0x3E, 0x41, 0x5D, 0x59, 0x4E}, // 64: @
    {0x7C, 0x12, 0x11, 0x12, 0x7C}, // 65: A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 66: B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 67: C
    {0x7F, 0x41, 0x41, 0x41, 0x3E}, // 68: D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 69: E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 70: F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 71: G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 72: H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 73: I
    {0x30, 0x40, 0x40, 0x40, 0x3F}, // 74: J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 75: K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 76: L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 77: M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 78: N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 79: O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 80: P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 81: Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 82: R
    {0x26, 0x49, 0x49, 0x49, 0x32}, // 83: S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 84: T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 85: U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 86: V
    {0x3F, 0x40, 0x30, 0x40, 0x3F}, // 87: W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 88: X
    {0x03, 0x04, 0x78, 0x04, 0x03}, // 89: Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 90: Z
    {0x00, 0x00, 0x7F, 0x41, 0x41}, // 91: [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // 92: \
    {0x41, 0x41, 0x7F, 0x00, 0x00}, // 93: ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // 94: ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // 95: _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // 96: `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 97: a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 98: b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 99: c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 100: d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 101: e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 102: f
    {0x08, 0x14, 0x14, 0x14, 0x7C}, // 103: g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 104: h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 105: i
    {0x20, 0x40, 0x40, 0x3D, 0x00}, // 106: j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 107: k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 108: l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // 109: m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 110: n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 111: o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 112: p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // 113: q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 114: r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 115: s
    {0x04, 0x3F, 0x3C, 0x04, 0x00}, // 116: t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 117: u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 118: v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 119: w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 120: x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 121: y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 122: z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // 123: {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // 124: |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // 125: }
    {0x10, 0x08, 0x10, 0x08, 0x10}  // 126: ~
};


// Função auxiliar para pulsar o Enable (EN)
void glcd_pulse_enable() {
    GLCD_EN = 1;
    __delay_us(5); // Atraso aumentado para maior segurança
    GLCD_EN = 0;
    __delay_us(5); // Atraso após o pulso
}

// Envia um comando ao GLCD
// O parâmetro 'chip' é para controle lógico interno da função, os CS reais são setados aqui
void glcd_command(uint8_t cmd, uint8_t chip) {
    // Configura pinos de controle
    GLCD_RS = 0; // Modo comando
    GLCD_RW = 0; // Modo escrita

    // Seleciona o chip (CS1 ou CS2)
    if (chip == 1) {
        GLCD_CS1 = 1;
        GLCD_CS2 = 0;
    } else { // chip == 2
        GLCD_CS1 = 0;
        GLCD_CS2 = 1;
    }

    // Coloca o comando no barramento de dados
    GLCD_DATA_PORT = cmd;
    // Pulsa o pino Enable para o comando ser lido
    glcd_pulse_enable();
    __delay_us(100); // Atraso maior após o comando para o controlador processar
}

// Envia dados ao GLCD
// O parâmetro 'chip' é para controle lógico interno da função, os CS reais são setados aqui
void glcd_data(uint8_t data, uint8_t chip) {
    // Configura pinos de controle
    GLCD_RS = 1; // Modo dados
    GLCD_RW = 0; // Modo escrita

    // Seleciona o chip (CS1 ou CS2)
    if (chip == 1) {
        GLCD_CS1 = 1;
        GLCD_CS2 = 0;
    } else { // chip == 2
        GLCD_CS1 = 0;
        GLCD_CS2 = 1;
    }

    // Coloca os dados no barramento
    GLCD_DATA_PORT = data;
    // Pulsa o pino Enable para os dados serem lidos
    glcd_pulse_enable();
    __delay_us(100); // Atraso maior após os dados
}

// Inicializa o GLCD
void glcd_init() {
    // Configura a direção dos pinos como SAÍDA (0)
    GLCD_RS_DIR = 0;
    GLCD_RW_DIR = 0;
    GLCD_EN_DIR = 0;
    GLCD_RST_DIR = 0;
    GLCD_CS1_DIR = 0;
    GLCD_CS2_DIR = 0;
    GLCD_DATA_TRIS = 0x00; // PORTD inteiro como saída

    // Sequência de Reset (assumindo RST é ativo baixo)
    GLCD_RST = 0; // Ativa Reset
    __delay_ms(50); // Atraso para garantir o reset
    GLCD_RST = 1; // Desativa Reset (para operação normal)
    __delay_ms(50); // Atraso após o reset para o controlador inicializar

    // Inicializa os dois chips do GLCD (KS0108)
    // Display ON (0x3F)
    glcd_command(0x3F, 1); // Display ON para CS1
    glcd_command(0x3F, 2); // Display ON para CS2

    // Set Start Line (0xC0) - Opcional, geralmente 0 por padrão
    glcd_command(0xC0, 1);
    glcd_command(0xC0, 2);

    // Limpa o display no final da inicialização
    glcd_clear();
}

// Limpa todo o display
void glcd_clear() {
    for (uint8_t page = 0; page < 8; page++) { // 8 páginas (0 a 7)
        for (uint8_t chip = 1; chip <= 2; chip++) { // Para cada um dos dois chips
            glcd_command(0xB8 | page, chip);    // Define a página (0xB8 é o comando "Set Page")
            glcd_command(0x40, chip);           // Define a coluna para 0 (0x40 é o comando "Set Column")

            for (uint8_t col = 0; col < 64; col++) { // 64 colunas por chip
                glcd_data(0x00, chip); // Escreve byte vazio (pixels apagados)
            }
        }
    }
}

// Define a página atual para escrita/leitura
void glcd_set_page(uint8_t page) {
    // Comando para setar a página é o mesmo para ambos os chips
    // Mas devemos enviá-lo para ambos, pois as operações subsequentes podem ser em qualquer um.
    glcd_command(0xB8 | page, 1); // Set Page para CS1
    glcd_command(0xB8 | page, 2); // Set Page para CS2
}

// Define a coluna atual para escrita/leitura e seleciona o chip correto
void glcd_set_column(uint8_t column) {
    if (column < 64) { // Colunas 0-63 (Chip 1)
        glcd_command(0x40 | column, 1); // Set Column para CS1
    } else { // Colunas 64-127 (Chip 2)
        glcd_command(0x40 | (column - 64), 2); // Set Column para CS2 (coluna relativa ao chip)
    }
}

// Escreve um byte de dados no local atual do cursor (página/coluna)
// A seleção do chip deve ter sido feita previamente por glcd_set_column ou manualmente.
void glcd_write_data(uint8_t data) {
    // glcd_data já cuida da seleção do chip através do parâmetro.
    // Esta função assume que o CS apropriado já foi selecionado pela última chamada a glcd_set_column.
    // É uma otimização, mas pode ser fonte de erro se o fluxo não for sempre "set_column -> write_data".
    // Para simplificar, vamos enviar para o chip que está "ativado" no hardware (CS1 ou CS2)
    if (GLCD_CS1 == 1) { // Verifica qual chip está logicamente ativo no hardware
        glcd_data(data, 1);
    } else {
        glcd_data(data, 2);
    }
}

// Exibe uma imagem bitmap completa no display (8 páginas x 128 colunas)
void glcd_display_image(const uint8_t *image) {
    for (uint8_t page = 0; page < 8; page++) {
        for (uint8_t chip = 1; chip <= 2; chip++) {
            glcd_command(0xB8 | page, chip);    // Define a página
            glcd_command(0x40, chip);           // Define a coluna para 0

            for (uint8_t col = 0; col < 64; col++) {
                glcd_data(*image++, chip); // Escreve o byte e avança o ponteiro da imagem
            }
        }
    }
}

// Desenha um caractere na posição especificada (baseado em font5x7)
void glcd_draw_char(char c, uint8_t page, uint8_t column) {
    // A fonte font5x7 começa com o caractere ESPAÇO (' ') no índice 0
    // e vai até o caractere TIL ('~') no índice 94.
    if (c < ' ' || c > '~') {
        c = ' '; // Se o caractere não for imprimível, desenha um espaço
    }
    
    // Calcula o índice do caractere na array font5x7
    // Ex: 'A' (ASCII 65) - ' ' (ASCII 32) = 33. Então 'A' está no índice 33 da fonte.
    const uint8_t *char_bitmap = font5x7[c - ' '];

    glcd_set_page(page); // Define a página para o caractere

    for (int i = 0; i < 5; i++) { // 5 bytes por caractere 5x7
        glcd_set_column(column + i); // Define a coluna para cada byte
        glcd_write_data(char_bitmap[i]); // Escreve o byte da fonte
    }
    // Adiciona uma coluna de espaço entre caracteres para melhor legibilidade
    glcd_set_column(column + 5);
    glcd_write_data(0x00); // Escreve um byte vazio (coluna de espaço)
}

// Escreve uma string de caracteres no display
void glcd_write_string(const char *str, uint8_t page, uint8_t start_column) {
    uint8_t current_column = start_column;
    while (*str != '\0') { // Percorre a string até o terminador nulo
        glcd_draw_char(*str, page, current_column); // Desenha cada caractere
        current_column += 6; // Avança a coluna para o próximo caractere (5 pixels + 1 de espaço)
        str++; // Próximo caractere na string
        
        // Verifica se precisa ir para o próximo chip ou próxima página
        if (current_column >= 128) { // Se exceder a largura total do display (128 colunas)
            current_column = 0; // Volta para o início da coluna
            page++;             // Próxima página
            if (page >= 8) {    // Se exceder as 8 páginas, para de escrever
                break;
            }
            glcd_set_page(page); // Atualiza a página no GLCD
        }
    }
}
