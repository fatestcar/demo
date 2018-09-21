cmake_minimum_required(VERSION 3.12)
project(EmbeddedSystemNJU2018_Demo)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib)

add_executable(EmbeddedSystemNJU2018_Demo
        "Basic Principle/GPIO/DCmotors.cpp"
        "Basic Principle/GPIO/decoderISR.cpp"
        "Basic Principle/GPIO/servo.cpp"
        examples/GPIO/DCmotors.cpp
        examples/GPIO/decoderISR.cpp
        examples/GPIO/servo.cpp
        examples/OpenCV/main.cpp
        lib/GPIOlib.h)
