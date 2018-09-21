//
// Created by 刘瑷玮 on 2018/9/21.
//


#include "GPIOlib.h"

using namespace GPIO;

int main(){

    init();

    controlLeft(FORWARD,50);
    controlRight(FORWARD,50);
    delay(1000);

    turnTo(45);

    delay(1000);

    turnTo(0);

    delay(10000);

    stopLeft();
    stopRight();

    return 0;

}