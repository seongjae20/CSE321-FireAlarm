/*
* Author: Seongjae Shin
* Purpose: To reveal a current temperature and turn on buzzer
* Extra modules/functions in file: -
* Assignment: CSE321 - Project 3
***********************************
* Inputs: A temperature to DHT11
* Outputs: LED, LCD, Buzzer
* Constraints: If temperature is too low, it shows responses.
***********************************
* references: 
*/

#include "mbed.h"
#include "DHT.h"

// set watchdog and limit time
Watchdog &watchMe = Watchdog::get_instance();
int speed = 2000; // 5s in each led state
#define wdTimeout 25000

// Call DHT11 class with Port B pin #2
DHT11 DHT11(PB_2);

// main() runs in its own thread in the OS
int main()
{
    // Enable Port B
    RCC->AHB2ENR |= 0x2;

    // Input MODER for DHT11 PB2
    GPIOB->MODER &= ~(0x30);

    // Outputs
    // PB15,PB13,PB12,PB11,PB10 for LEDs
    // PB6 for buzzer
    GPIOB->MODER &= ~(0x8AA02000);
    GPIOB->MODER |= 0x45501000;

    // start watchdog
    // print reset line then rest 5s (speed)
    watchMe.start(wdTimeout);
    printf("\n--------------------RESET--------------------");
    thread_sleep_for(speed);

    while (true) {
    // get input from DHT11
    // print out C, F, H
    DHT11.read();
    printf("\n");
    printf("Celsius: %d, Fahrenheit: %f, Humidifty: %d\r\n", DHT11.getCelsius(), DHT11.getFahrenheit(), DHT11.getHumidity());
    // Each range of temperature do something correctly
    if(DHT11.getCelsius() >= 0 && DHT11.getCelsius() <=10){
        GPIOB->ODR &= ~(0x2000 | 0x1000 | 0x800 | 0x400 | 0x40); // turn off 2nd~5th LEDs and buzzer
        GPIOB->ODR |= 0x8000; // turn on 1st LED
    }
    else if(DHT11.getCelsius() >= 11 && DHT11.getCelsius() <=20){
        GPIOB->ODR &= ~(0x8000 | 0x1000 | 0x800 | 0x400 | 0x40); // turn off buzzer and LEDs except 2nd one
        GPIOB->ODR |= 0x2000; // turn on 2nd LED
    }
    else if(DHT11.getCelsius() >= 21 && DHT11.getCelsius() <=30){
        GPIOB->ODR &= ~(0x8000 | 0x2000 | 0x800 | 0x400 | 0x40); // turn off buzzer and LEDs except 3rd one
        GPIOB->ODR |= 0x1000; // turn on 3rd LED
    }
    else if(DHT11.getCelsius() >= 31 && DHT11.getCelsius() <=40){
        GPIOB->ODR &= ~(0x8000 | 0x2000 | 0x1000 | 0x400 | 0x40); // turn off buzzer and LEDs except 4th one
        GPIOB->ODR |= 0x800; // turn on 4th LED
    }
    else if(DHT11.getCelsius() >= 41 && DHT11.getCelsius() <=50){
        GPIOB->ODR &= ~(0x8000 | 0x2000 | 0x1000 | 0x800 | 0x40); // turn off buzzer and LEDs except 5th one
        GPIOB->ODR |= 0x400; // turn on 5th LED
    }
    else if(DHT11.getCelsius() > 50){
        GPIOB->ODR |= (0x8000 | 0x2000 | 0x1000 | 0x800 | 0x400 | 0x40); // turn on all of LEDs and buzzer
    }
    thread_sleep_for(2000); // rest 2s
    }
}

