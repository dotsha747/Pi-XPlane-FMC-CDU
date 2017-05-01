################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Base64.cpp \
../src/CharsetTextures.cpp \
../src/ExtPlaneClient.cpp \
../src/Remote737FMCCDU2.cpp \
../src/Screen.cpp \
../src/TCPClient.cpp 

OBJS += \
./src/Base64.o \
./src/CharsetTextures.o \
./src/ExtPlaneClient.o \
./src/Remote737FMCCDU2.o \
./src/Screen.o \
./src/TCPClient.o 

CPP_DEPS += \
./src/Base64.d \
./src/CharsetTextures.d \
./src/ExtPlaneClient.d \
./src/Remote737FMCCDU2.d \
./src/Screen.d \
./src/TCPClient.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


