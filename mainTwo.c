// #include <stdio.h>
// #include <stdint.h>
// #include "pico/stdlib.h"
// #include "hardware/pwm.h"
// #include "hardware/clocks.h"
// #include "hardware/uart.h"

// #define SEG_A 0



// float clockDiv = 64;
// float wrap = 39062;
// bool direction = true;
// int currentMillis = 400;
// int servoPin = 18;
// int buzzer = 22;
// uint8_t segmentNumber[10] = {
//     0x3f,
//     0x06,
//     0x5b,
//     0x4f,
//     0x66,
//     0x6d,
//     0x7d,
//     0x07,
//     0x7f,
//     0x67,
// };

// uint8_t number = 0;

// uint8_t d0 = 1;
// uint8_t d1 = 2;
// uint8_t d2 = 4;
// uint8_t d3 = 5;

// uint8_t inputs[4] = {0,0,0,0};
// // uint8_t i1;
// // uint8_t i2;
// // uint8_t i3;
// // uint8_t i4;

// uint8_t clicks = 0;

// uint8_t KeyPadRow[4] = {14, 13, 12, 11};
// uint8_t KeyPadCol[4] = {10, 9, 8, 7};
// uint8_t KeyPad[4][4] = {{'1', '2', '3', 'A'},
//                         {'4', '5', '6', 'B'},
//                         {'7', '8', '9', 'C'},
//                         {'*', '0', '#', 'D'}};

// uint8_t GetKey(void) {
//     char key = 0;
//     gpio_put(buzzer,true);
//     sleep_ms(200);
//     gpio_put(buzzer,false);
//     while (1) {
//         int key_pressed = 0;
//         for (uint8_t i = 0; i < 4; i++) {
//             if (!gpio_get(KeyPadCol[i])) {
//                 key_pressed = 1;
//                 break;
//             }
//         }

//         if (!key_pressed) {
//             break;  
//         }
//     }

//     while (1) {
//         for (uint8_t w = 0; w < 4; w++) {
//             gpio_put(KeyPadRow[w], 0);  
//             for (uint8_t i = 0; i < 4; i++) {
//                 gpio_put(KeyPadCol[i], 1);  
//                 if (!gpio_get(KeyPadCol[i])) {
//                     while (!gpio_get(KeyPadCol[i]))
//                         ;
//                     key = KeyPad[w][i];
//                     break;
//                 }
//             }
//             gpio_put(KeyPadRow[w], 1);  
//             if (key != 0) {
//                 break;
//             }
//         }

//         if (key != 0) {
//             break;
//         }
//     }

//     return key;
// }

// void setMillis(int servoPin, float millis)
// {
//     pwm_set_gpio_level(servoPin, (millis/20000.f)*wrap);
// }

// void setServo(int servoPin, float startMillis)
// {
//     gpio_set_function(servoPin, GPIO_FUNC_PWM);
//     uint slice_num = pwm_gpio_to_slice_num(servoPin);

//     pwm_config config = pwm_get_default_config();
    
//     uint64_t clockspeed = clock_get_hz(5);
//     clockDiv = 64;
//     wrap = 39062;

//     while (clockspeed/clockDiv/50 > 65535 && clockDiv < 256) clockDiv += 64; 
//     wrap = clockspeed/clockDiv/50;

//     pwm_config_set_clkdiv(&config, clockDiv);
//     pwm_config_set_wrap(&config, wrap);

//     pwm_init(slice_num, &config, true);

//     setMillis(servoPin, startMillis);
// }

// void initUART() {
//     uart_init(uart0, 9600);
//     gpio_set_function(16, GPIO_FUNC_UART);
//     gpio_set_function(17, GPIO_FUNC_UART);
//     uart_set_hw_flow(uart0, false, false);
//     uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
// }

// void sendUARTMessage(const char *message) {
//     while (*message) {
//         uart_putc(uart0, *message++);
//     }
// }
// char receiveUARTMessage() {
//      return uart_getc(uart0);
// }

// int main() {
//     stdio_init_all();

//     initUART();

//     gpio_init(20);
//     gpio_init(21);
//     gpio_init(buzzer);

//     gpio_set_dir(20, GPIO_OUT);
//     gpio_set_dir(21, GPIO_OUT);
//     gpio_set_dir(buzzer, GPIO_OUT);

//     for (uint8_t i = SEG_A; i < SEG_A + 7; i++) {
//         gpio_init(i);
//         gpio_set_dir(i, GPIO_OUT);
//     }

//     for (uint8_t i = 0; i < 4; i++) {
//         gpio_init(KeyPadRow[i]);
//         gpio_set_dir(KeyPadRow[i], GPIO_OUT);
//         gpio_init(KeyPadCol[i]);
//         gpio_set_dir(KeyPadCol[i], GPIO_IN);
//         gpio_pull_up(KeyPadCol[i]);
//     }

    
//     bool keypadActive = false;
//     while (1) {
//       if(uart_is_readable(uart0)){
//         char receivedChar = uart_getc(uart0);
//         if (receivedChar == 'D') {
//             printf("%s \n", "Deactivated");
//             clicks = 0;
//             keypadActive = false;
//             for (uint8_t i = SEG_A; i < SEG_A + 7; i++) {
//               gpio_put(i, false);
//             }
//         }
//         else if(receivedChar == 'A'){
//           printf("%s \n", "Activated");
//           keypadActive = true;
//         }
//       }

//       if(keypadActive){
//           char key = GetKey();
//           if (key >= '0' && key <= '9') {
//               number = key - '0';
//           } else {
//               number = 0;
//           }

//           uint8_t output = segmentNumber[number];

//           for (uint8_t i = SEG_A; i < SEG_A + 7; i++) {
//               gpio_put(i, (output & (1 << (i - SEG_A))) ? 1 : 0);
//           }
//           inputs[clicks] = number;
//           printf("input of %d = %d \n",clicks,inputs[clicks]);
//           clicks++;
//           printf("clicks %d \n",clicks);
//           if(clicks > 3){
//               if(d0 == inputs[0] && d1 == inputs[1] && d2 == inputs[2] && d3 == inputs[3]){
//                   printf("Pin correct! d0 %d d1 %d d2 %d d3 %d \n", inputs[0],inputs[1],inputs[2],inputs[3]);
//                   char message[] = "Pin correct!";
//                   uart_putc(uart0, 'Y');
//                   printf("Sent: %s\n", message);
              
//                   for (int i = 0; i < 10; i++)
//                   {
//                       gpio_put(20,true);
//                       sleep_ms(200);
//                       gpio_put(20,false);
//                       sleep_ms(200);            
//                   }
//                   setServo(servoPin, currentMillis);
//                   uint64_t start_time = time_us_64();
//                   while (true)
//                   {
//                       currentMillis += (direction)?5:-5;
//                       if (currentMillis >= 2400) direction = false;
//                       if (currentMillis <= 400) direction = true;
//                       setMillis(servoPin, currentMillis);
//                       sleep_ms(1);
//                       uint64_t elapsed_time = time_us_64() - start_time;
//                       if (elapsed_time >= 3000000) {
//                           gpio_put(servoPin,false);
//                           uart_putc(uart0, 'R');
//                           break;
//                       }
//                   }
//               }
//               else{
//                   printf("Incorrect pin! d0 %d d1 %d d2 %d d3 %d \n", inputs[0],inputs[1],inputs[2],inputs[3]);
//                   char message[] = "Incorrect pin!";
//                   uart_putc(uart0, 'N');
//                   printf("Sent: %s\n", message);
//                   for (int i = 0; i < 10; i++){
//                           gpio_put(21,true);
//                           gpio_put(buzzer,true);
//                           sleep_ms(200);
//                           gpio_put(21,false);
//                           gpio_put(buzzer,false);
//                           sleep_ms(200);            
//                   }
//               }
//               clicks = 0;
//           }
//       }
         
//     }

//     return 0;
// }
