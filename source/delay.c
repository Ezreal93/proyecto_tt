#include "delay.h"

#define DELAY_FACTOR 1

void delay_ms(int delay_time){
    for(int i = 0; i < delay_time ; i++){
        for(int j = 0; j < (1000*DELAY_FACTOR) ; j++){
        }
    }
}
