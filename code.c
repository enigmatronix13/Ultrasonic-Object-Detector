// Obstacle Detector using LPC1768 and HC-SR04
// Measures distance and displays on LCD; LED ON if distance < 30cm

#include <LPC17xx.h>
#include <stdio.h>  // For sprintf

#define TRIG_PIN 15     // P0.15 - Trigger
#define ECHO_PIN 16     // P0.16 - Echo
#define LED_PIN 8       // P0.8  - LED

// LCD pins
#define RS (1 << 27)    
#define EN (1 << 28)    
#define LCD_DATA_MASK 0x07800000  // P0.23-P0.26

char buffer[16];
int i;

// Microsecond delay using Timer1
void delay_us(uint32_t us) {
    LPC_TIM1->TCR = 0x02; 
    LPC_TIM1->PR = 99; 
    LPC_TIM1->TCR = 0x01;
    while (LPC_TIM1->TC < us);
    LPC_TIM1->TCR = 0x00;
}

// Millisecond delay
void delay_ms(uint32_t ms) {
    int i;
    for (i = 0; i < ms; i++) delay_us(1000);
}

// GPIO setup
void init_GPIO() {
    LPC_GPIO0->FIODIR |= (1 << TRIG_PIN) | (1 << LED_PIN);
    LPC_GPIO0->FIODIR &= ~(1 << ECHO_PIN);
    LPC_GPIO0->FIODIR |= RS | EN | LCD_DATA_MASK;
}

// Timer1 setup
void init_timer1() {
    LPC_SC->PCONP |= (1 << 2);
    LPC_TIM1->TCR = 0x00;
    LPC_TIM1->PR = 0;
    LPC_TIM1->TCR = 0x02;
}

// Measure distance (cm)
uint32_t measure_distance_cm() {
    uint32_t duration;

    LPC_GPIO0->FIOCLR = (1 << TRIG_PIN);
    delay_us(2);
    LPC_GPIO0->FIOSET = (1 << TRIG_PIN);
    delay_us(10);
    LPC_GPIO0->FIOCLR = (1 << TRIG_PIN);

    while (!(LPC_GPIO0->FIOPIN & (1 << ECHO_PIN)));
    LPC_TIM1->TCR = 0x02;
    LPC_TIM1->TCR = 0x01;
    while (LPC_GPIO0->FIOPIN & (1 << ECHO_PIN));
    LPC_TIM1->TCR = 0x00;
    duration = LPC_TIM1->TC;

    return (duration * 343) / (2 * 10000);
}

// LCD enable pulse
void lcd_enable_pulse() {
    LPC_GPIO0->FIOSET = EN;
    delay_us(10);
    LPC_GPIO0->FIOCLR = EN;
    delay_us(10);
}

// Send 4 bits to LCD
void lcd_send_nibble(unsigned int nibble) {
    LPC_GPIO0->FIOCLR = LCD_DATA_MASK;
    LPC_GPIO0->FIOSET = (nibble << 23) & LCD_DATA_MASK;
    lcd_enable_pulse();
}

// Send command to LCD
void lcd_cmd(unsigned char cmd) {
    LPC_GPIO0->FIOCLR = RS;
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd & 0x0F);
    delay_ms(2);
}

// Send data to LCD
void lcd_data(unsigned char data) {
    LPC_GPIO0->FIOSET = RS;
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    delay_ms(2);
}

// LCD initialization
void lcd_init() {
    delay_ms(20);
    lcd_send_nibble(0x03);
    delay_ms(5);
    lcd_send_nibble(0x03);
    delay_us(100);
    lcd_send_nibble(0x03);
    lcd_send_nibble(0x02);

    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

// Print string on LCD
void lcd_print(char *msg) {
    int i = 0;
    while (msg[i]) lcd_data(msg[i++]);
}

// Main loop
int main() {
    SystemInit();
    init_GPIO();
    init_timer1();
    lcd_init();

    while (1) {
        uint32_t distance = measure_distance_cm();

        lcd_cmd(0x01);
        lcd_cmd(0x80);

        sprintf(buffer, "Distance: %dcm", distance);
        lcd_print(buffer);

        if (distance > 0 && distance < 30) {
            LPC_GPIO0->FIOSET = (1 << LED_PIN);
        } else {
            LPC_GPIO0->FIOCLR = (1 << LED_PIN);
        }

        delay_ms(300);
    }
}
