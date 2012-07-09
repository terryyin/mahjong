CPPUTEST_ROOT := cpputest
COMPONENT_ROOT := .
COMPONENT_SRC_DIRS := src src/mahjong_game src/html_ui src/exercise
COMPONENT_TST_DIRS := tst tst/private

COMPONENT_UT := mahjong_ut
COMPONENT_SRC_LIB := libMahjong.a
COMPONENT_UT_LDFLAGS := -lpthread  

COMPONENT_PRODUCT := mahjong
COMPONENT_PRODUCT_LIBS := ./mahjong_evaluator/lib/libMahjongEvaluator.a
COMPONENT_PRODUCT_LDFLAGS := -lpthread -lstdc++
ifeq ($(OS),Windows_NT)
COMPONENT_PRODUCT_LDFLAGS += -lws2_32
CC = gcc
endif

COMPONENT_INCLUDE := -I./include
COMPONENT_CFLAGS	:= -ggdb -Wall -Werror -DEVALUATOR=TERRY_EVALUATOR
COMPONENT_CXXFLAGS	:= $(COMPONENT_CFLAGS)

include default.mk

#extra targets
.PHONY: product clean_product evaluator_ut clean_evaluator
evaluator_ut: $(CPPUTEST_LIB)
	@make -C mahjong_evaluator

$(COMPONENT_PRODUCT_LIBS):
	make -C mahjong_evaluator product

clean_evaluator:
	@make -C mahjong_evaluator clean

all: evaluator_ut
clean: clean_product clean_evaluator
