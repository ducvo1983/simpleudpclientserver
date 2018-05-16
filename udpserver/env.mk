################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 

ifeq ($(BOOST_INCLUDEDIR),)
	BOOST_INCLUDEDIR:=.
endif

ifeq ($(BOOST_LIBRARYDIR),)
	BOOST_LIBRARYDIR:=.
endif
PTHREAD:=
PLATFORM:=$(shell uname -s)

ifneq ($(PLATFORM),Darwin)
	PTHREAD += -lpthread
endif
