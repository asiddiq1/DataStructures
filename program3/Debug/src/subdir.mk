################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/driver.cpp \
../src/test_map.cpp \
../src/test_priority_queue.cpp \
../src/wordgenerator.cpp 

OBJS += \
./src/driver.o \
./src/test_map.o \
./src/test_priority_queue.o \
./src/wordgenerator.o 

CPP_DEPS += \
./src/driver.d \
./src/test_map.d \
./src/test_priority_queue.d \
./src/wordgenerator.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	/usr/local/bin/g++-5 -std=c++11 -I"/Users/aishasiddiq/Documents/ics46workspace/courselib/src" -I"/Users/aishasiddiq/Documents/ics46workspace/googletestlib/include" -O0 -g3 -gdwarf-3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


