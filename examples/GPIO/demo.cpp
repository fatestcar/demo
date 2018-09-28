//
// Created by 刘瑷玮 on 2018/9/21.
//


#include "GPIOlib.h"

using namespace GPIO;

int main(){

    init();
    turnTo(0);
    delay(1000);
    controlLeft(FORWARD,4);
    controlRight(FORWARD,4);
    delay(1000);

    delay(1000);
    delay(1000000);
    stopLeft();
    stopRight();
    return 0;

}