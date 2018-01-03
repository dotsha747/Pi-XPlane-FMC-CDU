################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Base64.cpp \
../src/CharsetTextures.cpp \
../src/ExtPlaneClient.cpp \
../src/FMCList.cpp \
../src/KeypadScanner.cpp \
../src/LEDs.cpp \
../src/PiXPlaneFMCCDU.cpp \
../src/Screen.cpp \
../src/TCPClient.cpp \
../src/X737FMC.cpp \
../src/XPlaneNetworkClient.cpp \
../src/ZeroFMC.cpp \
../src/ZiboFMC.cpp 

OBJS += \
./src/Base64.o \
./src/CharsetTextures.o \
./src/ExtPlaneClient.o \
./src/FMCList.o \
./src/KeypadScanner.o \
./src/LEDs.o \
./src/PiXPlaneFMCCDU.o \
./src/Screen.o \
./src/TCPClient.o \
./src/X737FMC.o \
./src/XPlaneNetworkClient.o \
./src/ZeroFMC.o \
./src/ZiboFMC.o 

CPP_DEPS += \
./src/Base64.d \
./src/CharsetTextures.d \
./src/ExtPlaneClient.d \
./src/FMCList.d \
./src/KeypadScanner.d \
./src/LEDs.d \
./src/PiXPlaneFMCCDU.d \
./src/Screen.d \
./src/TCPClient.d \
./src/X737FMC.d \
./src/XPlaneNetworkClient.d \
./src/ZeroFMC.d \
./src/ZiboFMC.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


