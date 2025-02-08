#include <stdio.h>
#include "pico/stdlib.h"
#include <stdlib.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"

//inclusão do arquivo pio
#include "uart.pio.h"

//variáveis da Matriz de leds
#define NUM_PIXELS 25 //quantidade de leds na matriz
#define OUT_PIN 7 //gpio da matriz de leds
//botões e led rgb
const uint led_red = 13; //gpio do led rgb vermelho
const uint buttonA = 5; // gpio do botão 1
const uint buttonB = 6; // gpio do botão 2

int numero_atual = 0;
// variáveis para executar funções de interrupção e debouncing
volatile uint32_t last_time_A = 0;
volatile uint32_t last_time_B = 0;
// variáveis para contagem da mudança de estado dos butões
static volatile uint a = 1;
static volatile uint b = 1;

// Prototipação da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica o debounce para o botão A
    if (gpio == buttonA && current_time - last_time_A > 200000) { // 200 ms de debounce
        last_time_A = current_time; // muda o último estado para atual
        printf("Botão A = %d\n", a);
        // Ação para o botão A
        if (!gpio_get(buttonA) && numero_atual < 9 ) {
                numero_atual++; //Incrementa o número atual
                a++; // aumenta a quantidade de vezes que o botão foi pressinado     
        }
    }
    // Verifica o debounce para o botão B
    if (gpio == buttonB && current_time - last_time_B > 200000) {
        last_time_B = current_time; // muda o último estado para atual
        printf("Botão B = %d\n", b);
        // Ação para o botão B
        if (!gpio_get(buttonB) && numero_atual > 0) {
                numero_atual--; // Decrementa o número atual
                b++; //umenta quantidade de vezes que o botão foi pressinado
            
        }
    }
}

//função para piscar led vermelho 5 vezes por segundo
void blink_red_led(){
  gpio_put(led_red, true);
  sleep_ms(100);
  gpio_put(led_red, false);
  sleep_ms(100);
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

    for (int i = 0; i < 25; i++) {
        valor_led = matrix_rgb(0.0, 0.0, matriz_atual[24 - i]); // led verde sendo utilizado
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

//inicar led vermelho
gpio_init(led_red);
gpio_set_dir(led_red, GPIO_OUT);

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
     blink_red_led();

    exibir_numero(pio, sm, numero_atual);
    }
}



    
    
