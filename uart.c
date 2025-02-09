#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

//inclusão do arquivo pio
#include "uart.pio.h"

//variáveis da Matriz de leds
#define NUM_PIXELS 25 //quantidade de leds na matriz
#define OUT_PIN 7 //gpio da matriz de leds

//display
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

//botões e led rgb
const uint led_green = 11; //gpio do led rgb verde
const uint led_blue = 12; //gpio do led rgb azul
const uint buttonA = 5; // gpio do botão 1
const uint buttonB = 6; // gpio do botão 2

int numero_atual = 0;// variável útil para fazer a exibição dos números

// variáveis para executar funções de interrupção e debouncing
volatile uint32_t last_time_A = 0;
volatile uint32_t last_time_B = 0;

// Variáveis globais para armazenar o estado dos LEDs
bool estado_led_green = false; //  desligado
bool estado_led_blue = false;  //  desligado

// Prototipação da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Verifica o debounce para o botão A
    if (gpio == buttonA && current_time - last_time_A > 200000) { // 200 ms de debounce
        last_time_A = current_time; // muda o último estado para atual    
        // Ação para o botão A
        if (!gpio_get(buttonA)  ) {
            estado_led_green = !estado_led_green; // Alterna o estado
            gpio_put(led_green, estado_led_green); // Atualiza o led
            printf("O led verde mudou de estado.\n");      
        }
    }
    // Verifica o debounce para o botão B
    if (gpio == buttonB && current_time - last_time_B > 200000) {
        last_time_B = current_time; // muda o último estado para atual        
        // Ação para o botão B
        if (!gpio_get(buttonB)) {
            estado_led_blue = !estado_led_blue; // Alterna o estado
            gpio_put(led_blue, estado_led_blue); // Atualiza o led
            printf("O led azul mudou de estado.\n");
        }
    }
}
// representação dos números de 0-9
double numeros[10][25] = {
      {
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0},

      {
        0.0, 0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.5, 0.0,
        0.0, 0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0},    

      {
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.0, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0}, 

     {
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0}, 

     {
        0.0, 0.5, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.5, 0.0},

     {
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.0, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0},
        
     {
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.0, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0},

     {
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0, 0.0},

     {
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0},

     {
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.5, 0.0,
        0.0, 0.5, 0.5, 0.5, 0.0,
        0.0, 0.5, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.5, 0.0},
};

//definir intensidade do led
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

void inicializar_clock() {
    bool ok = set_sys_clock_khz(128000, false);
    if (ok) {
        printf("Clock set to %ld\n", clock_get_hz(clk_sys));
    } else {
        printf("Falha ao configurar o clock\n");
    }
}

// Configura a PIO
void configurar_pio(PIO pio, uint *offset, uint *sm) {
    *offset = pio_add_program(pio, &uart_program);
    *sm = pio_claim_unused_sm(pio, true);
    uart_program_init(pio, *sm, *offset, OUT_PIN);
}
//função para exibição do número atual na matriz de leds
    void exibir_numero(PIO pio, uint sm, int numero) {
    uint32_t valor_led;
    double* matriz_atual = numeros[numero]; // Ponteiro para a matriz do número atual

// Exibe o número no terminal 
     printf("Exibindo o número: %d\n", numero);

    for (int i = 0; i < 25; i++) {
        valor_led = matrix_rgb(0.0, matriz_atual[24 - i], 0.0); // led vermelho sendo utilizado
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

int main(){
 PIO pio = pio0;
 uint offset, sm;
 uint32_t valor_led;
 double r = 0.0, b = 0.0, g = 0.0;
    
 stdio_init_all();
 inicializar_clock();
 configurar_pio(pio, &offset, &sm);

// Inicia o I2C com 400Khz.
i2c_init(I2C_PORT, 400 * 1000);
// Defini os pinos GPIO I2C
gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); 
gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); 
gpio_pull_up(I2C_SDA); 
gpio_pull_up(I2C_SCL); 
ssd1306_t ssd; // Inicializa a estrutura do display
ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
ssd1306_config(&ssd); // Configura o display
ssd1306_send_data(&ssd); // Envia os dados para o display
 
// Limpa o display. O display inicia com todos os pixels apagados.
ssd1306_fill(&ssd, false);
ssd1306_send_data(&ssd);

//inicar led verde e azul
gpio_init(led_green);
gpio_set_dir(led_green, GPIO_OUT);

gpio_init(led_blue);
gpio_set_dir(led_blue, GPIO_OUT);

//Inicia butões
gpio_init(buttonA);
gpio_set_dir(buttonA, GPIO_IN);
gpio_pull_up(buttonA);

gpio_init(buttonB);
gpio_set_dir(buttonB, GPIO_IN);
gpio_pull_up(buttonB);

// aplicando interruções e suas funcionalidade
gpio_set_irq_enabled_with_callback(buttonA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
gpio_set_irq_enabled_with_callback(buttonB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

while(true){
    if(gpio_get(buttonA) == 0){
        // Atualiza o display SSD1306
        ssd1306_fill(&ssd, false); // Limpa o display
        ssd1306_draw_string(&ssd, estado_led_green ? "led verde: ON" : "led verde:OFF", 18, 24); // exibe mensagem informativa no display
        ssd1306_send_data(&ssd); // Atualiza o displa
    }
    if(gpio_get(buttonB) == 0){
        // Atualiza o display SSD1306
        ssd1306_fill(&ssd, false); // Limpa o display
        ssd1306_draw_string(&ssd, estado_led_blue ? "led azul: ON" : "led azul:OFF", 18, 24); // exibe mensagem informativa no display
        ssd1306_send_data(&ssd); // Atualiza o displa
    }

    if (stdio_usb_connected()){
        char c;
        if (scanf("%c", &c) == 1){
         // Lê caractere da entrada padrão
            printf("Recebido: '%c'\n", c);

        // Exibe o caractere no display 
        ssd1306_fill(&ssd, false);  // Limpa o display
        ssd1306_draw_char(&ssd, c, 56, 24);  // Mostra o caractere recebido
        ssd1306_send_data(&ssd);  // Atualiza o display

     switch (c) {
        case '0':
            numero_atual = 0;
            break;
        case '1':
            numero_atual = 1;
            break;
        case '2':
            numero_atual = 2;
            break;
        case '3':
            numero_atual = 3;
            break;
        case '4':
            numero_atual = 4;
            break;
        case '5':
            numero_atual = 5;
            break;
        case '6':
            numero_atual = 6;
            break;
        case '7':
            numero_atual = 7;
            break;
        case '8':
            numero_atual = 8;
            break;
        case '9':
            numero_atual = 9;
            break;
        default:
            printf("Caractere inválido: %c\n", c); // Caso o caractere não seja um número
            break;
    }
    // Exibe o número selecionado na matriz de LEDs
    exibir_numero(pio, sm, numero_atual);
}
    }
    sleep_ms(40);
}
}



    
    
