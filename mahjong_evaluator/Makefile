CPPUTEST_HOME := ../cpputest
EVALUATOR = mahjong_evaluator
CPPUTEST_WARNINGFLAGS =  -Wall -Wextra -Wshadow -Wswitch-default -Wswitch-enum -Wconversion
#Set this to @ to keep the makefile quiet
ifndef SILENCE
	SILENCE = @
endif

#--- Inputs ----#
COMPONENT_NAME = $(EVALUATOR)

CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Gcc

SRC_DIRS = \
	src
TEST_SRC_DIRS = \
	tst

INCLUDE_DIRS =\
  .\
  include\
  ../include\
  $(SRC_DIRS)\
  $(CPPUTEST_HOME)/include
  
include $(CPPUTEST_HOME)/build/MakefileWorker.mk

