//This example uses the ultrasonic script to get centimeters and writes it to UART

#include "pico/stdlib.h"
#include <stdio.h>
#include "ultrasonic.h"
#include "hardware/uart.h"
#include "LCDops.h"
#include "generalOps.h"
#include "presetChars.h"
#include "presetMessages.h"
#include "hardware/uart.h"


uint trigPin = 11;
uint echoPin = 12;
#define SENSOR_PIN 2
int ultrasonicDistance;
char currentChar = 'A';

int LCDpins[14] = {3,4,5,6,7,8,9,10,15,16,17,16,2};

char receiveUARTMessage() {
    return uart_getc(uart0);
}

void initUltrasonic(){
stdio_init_all();
    setupUltrasonicPins(trigPin, echoPin);
    while (true) { 
        ultrasonicDistance = getCm(trigPin, echoPin);
        printf("\n %d cm", ultrasonicDistance);

        if (ultrasonicDistance <= 4)
        {
            printf("you can enter the pin now %d", ultrasonicDistance);
            LCDclear();
            sleep_ms(8);
            LCDgoto("00");
            LCDsendRawInstruction(0,0,"00001100");
            LCDwriteMessage("Enter your pin");
            if(currentChar == 'D'){
                uart_putc(uart0, 'A');
                printf("Sent char : %s","A");
                currentChar = 'A';
            }
            
            printf("sent %s","A");
            sleep_ms(500);
            while (1) {
                char receivedChar;
                if(getCm(trigPin,echoPin) > 4){
                    break;
                }
                // Check if a character is available before receiving
                if (uart_is_readable(uart0)) {
                    receivedChar = receiveUARTMessage();
                    printf("Received: %c\n", receivedChar);
                    if(receivedChar == 'Y'){
                        LCDclear();
                        sleep_ms(8);
                        LCDgoto("00");
                        LCDsendRawInstruction(0,0,"00001100");
                        LCDwriteMessage("Pin correct");
                        sleep_ms(5000);
                        LCDclear();

                        sleep_ms(8);
                        LCDgoto("00");
                        LCDsendRawInstruction(0,0,"00001100");
                        LCDwriteMessage("Remove your card");
                        sleep_ms(5000);
                        LCDclear();
                        break;
                    }
                    else if (receivedChar == 'N')
                    {
                        LCDclear();
                        sleep_ms(8);
                        LCDgoto("00");
                        LCDsendRawInstruction(0,0,"00001100");
                        LCDwriteMessage("Incorrect pin");
                        sleep_ms(5000);
                        LCDclear();
                        break;
                    }
                }
                
            }
            
        }
        else{
            LCDclear();
            if(currentChar == 'A'){
                uart_putc(uart0, 'D');
                printf("Sent char : %s","D");
                currentChar = 'D';
            }
            
        }
        sleep_ms(500);
    }
}

void initInfraredSensor(){
    gpio_init(SENSOR_PIN);
    gpio_set_dir(SENSOR_PIN, GPIO_IN);
    // Main loop
    while (1) {
        int sensor_value = gpio_get(SENSOR_PIN);
        sleep_ms(500);
    }
}

initLcd(){

    stdio_init_all();

    for(int gpio = 0; gpio < 11; gpio++){
        gpio_init(LCDpins[gpio]);
        gpio_set_dir(LCDpins[gpio], true);
        gpio_put(LCDpins[gpio], false);
    }

    LCDinit();
    
    
}

void initUART() {
    uart_init(uart0, 9600);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
    uart_set_hw_flow(uart0, false, false);
    uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
}



int main()
{
    
    initUART();
    

    initLcd();
    initUltrasonic();
    initInfraredSensor();
    
    
    
}