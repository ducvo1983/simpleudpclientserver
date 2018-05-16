################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
./UDPServer.cpp 

OBJS += \
./UDPServer.o 

CPP_DEPS += \
./UDPServer.d 


# Each subdirectory must supply rules for building sources it contributes
./%.o: ./%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -I $(BOOST_INCLUDEDIR) -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" $(PTHREAD)
	@echo 'Finished building: $<'
	@echo ' '


