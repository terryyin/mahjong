CPPUTEST_ROOT := ../cpputest
COMPONENT_ROOT := .
COMPONENT_SRC_DIRS := src include
COMPONENT_TST_DIRS := tst

COMPONENT_UT := mj_evaluator_ut
COMPONENT_SRC_LIB := libMahjongEvaluatorUT.a
COMPONENT_LIB := lib/libMahjongEvaluator.a

LDFLAGS = -lpthread  
COMPONENT_CFLAGS	:=  -ggdb -Wall -I src -I../include
COMPONENT_CXXFLAGS	:= $(COMPONENT_CFLAGS)
ifeq ($(OS),Windows_NT)
CC = gcc
endif

include default.mk

#extra targets
.PHONY: product clean_product
product: $(COMPONENT_LIB)

src_to_o = $(subst .c,.o,$(subst .cpp,.o,$1))
SRC_OBJS := $(foreach f,$(COMPONENT_ALL_SRC),$(call src_to_o,$f))

$(COMPONENT_LIB): $(SRC_OBJS)
	@$(AR) -r $@ $^

clean: clean_product

clean_product:
	-$(RM) $(COMPONENT_LIB) $(SRC_OBJS)
