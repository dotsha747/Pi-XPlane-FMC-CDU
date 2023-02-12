cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(Threads REQUIRED)


find_package(SDL2 REQUIRED)
include_directories("/home/lefteris/Projects/pi/rootfs/usr/include" ${SDL2_INCLUDE_DIRS} )

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    message(STATUS "Building for x86_64 architecture")
    set(PLATFORM "x86")
else()
    set(PLATFORM arm)
    message(STATUS "Building for ARM architecture")
endif()

add_executable(pi-FMC AbstractFMC.h FMCManager.h FMCManager.cpp platform/KeypadScanner.h platform/${PLATFORM}/KeypadScanner.h platform/${PLATFORM}/KeypadScanner.cpp 
                LEDs.h LEDs.cpp MainFMC.h MainFMC.cpp  PiXPlaneFMCCDU.cpp Screen.h Screen.cpp 
                SplashFMC.h SplashFMC.cpp X737FMC.h X737FMC.cpp XfmcFMC.h XfmcFMC.cpp ZiboFMC.h ZiboFMC.cpp RotaryEncScanner.cpp RotaryEncScanner.h)


target_link_libraries(pi-FMC 
    Threads::Threads
    ${SDL2_LIBRARIES}
    SDL2_ttf
    freetype 
    z 
    png 
    XPlaneUDPClient 
    wiringPi 
    XPlaneExtPlaneClient
    )


    install(TARGETS pi-FMC RUNTIME)