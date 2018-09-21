cmake_minimum_required(VERSION 3.12)
project(demo)

set(CMAKE_CXX_STANDARD 14)

include_directories(examples/GPIO)
include_directories(lib)

add_executable(demo
        "Basic Principle/GPIO/DCmotors.cpp"
        "Basic Principle/GPIO/decoderISR.cpp"
        "Basic Principle/GPIO/servo.cpp"
        examples/GPIO/DCmotors.cpp
        examples/GPIO/decoderISR.cpp
        examples/GPIO/GPIOlib.h
        examples/GPIO/servo.cpp
        examples/OpenCV/main.cpp
        lib/GPIOlib.h examples/GPIO/demo.cpp)
