ifeq "$(MAKECMDGOALS)" "product"
OBJ_ROOT := $(COMPONENT_ROOT)/.product
else
OBJ_ROOT := $(COMPONENT_ROOT)/.ut
endif

.PHONY: all product clean
all: $(COMPONENT_UT)
	./$(COMPONENT_UT)
#	$(foreach x, $(ALL_SRC_DIRS), gcov -o $(OBJ_ROOT)/$x $x/*.c;)
	@find .ut -name "*.gcda" -exec rm {} \;

product: $(COMPONENT_PRODUCT)
	./$^

clean:
	-$(RM) $(COMPONENT_UT) $(COMPONENT_PRODUCT)
	-$(RM) $(COMPONENT_SRC_LIB) $(COMPONENT_STUB_LIB)
	-$(RM) -r $(COMPONENT_ROOT)/.product $(COMPONENT_ROOT)/.ut $(COMPONENT_ROOT)/gmon.out
	-$(RM) -r $(COMPONENT_ROOT)/mahjong_evaluator/src/*.gcda
	-$(RM) -r $(COMPONENT_ROOT)/mahjong_evaluator/src/*.gcno
	-$(RM) -r $(COMPONENT_ROOT)/mahjong_evaluator/tst/*.gcda
	-$(RM) -r $(COMPONENT_ROOT)/mahjong_evaluator/tst/*.gcno
	-$(RM) -r $(COMPONENT_ROOT)/cpputest/CppUTest_tests
	-$(RM) -r $(COMPONENT_ROOT)/cpputest/CppUTestExt_tests
	-$(RM) -r $(COMPONENT_ROOT)/cpputest/objs

ALL_SRC_DIRS := $(foreach x,$(COMPONENT_SRC_DIRS),$(COMPONENT_ROOT)/$x)
ALL_TST_DIRS := $(foreach x,$(COMPONENT_TST_DIRS),$(COMPONENT_ROOT)/$x)
ALL_STUB_DIRS:= $(foreach x,$(COMPONENT_STUB_DIRS),$(COMPONENT_ROOT)/$x)

get_src = $(wildcard $1/*.cpp) $(wildcard $1/*.c)
ALL_SRC := $(foreach x,$(ALL_SRC_DIRS),$(call get_src,$x))
ALL_TST := $(foreach x,$(ALL_TST_DIRS),$(call get_src,$x))
ALL_STUB:= $(foreach x,$(ALL_STUB_DIRS),$(call get_src,$x))

src_to_obj = $(patsubst $(COMPONENT_ROOT)/%.c,$(OBJ_ROOT)/%.obj,$(patsubst $(COMPONENT_ROOT)/%.cpp,$(OBJ_ROOT)/%.obj,$1))
ALL_SRC_OBJS := $(foreach f,$(ALL_SRC),$(call src_to_obj,$f))
ALL_TST_OBJS := $(foreach f,$(ALL_TST),$(call src_to_obj,$f))
ALL_STUB_OBJS:= $(foreach f,$(ALL_STUB),$(call src_to_obj,$f))

ALL_DEP := $(ALL_SRC_OBJS:.obj=.d)
ALL_INCLUDE := $(COMPONENT_INCLUDE) $(foreach x,$(ALL_SRC_DIRS),-I $x)

ifneq "$(MAKECMDGOALS)" "product"
CPPUTEST_LIB := $(CPPUTEST_ROOT)/lib/libCppUTest.a $(CPPUTEST_ROOT)/lib/libCppUTestExt.a
$(CPPUTEST_LIB):
	$(SILENCE)$(MAKE) -e -C $(CPPUTEST_ROOT) extensions

ALL_INCLUDE := $(ALL_INCLUDE) -I$(CPPUTEST_ROOT)/include \
               $(foreach x,$(ALL_TST_DIRS),-I $x) \
               $(foreach x,$(ALL_STUB_DIRS),-I $x)

COMPONENT_CFLAGS += -DUNIT_TEST -include CppUTest/MemoryLeakDetectorMallocMacros.h 
COMPONENT_CXXFLAGS += -DUNIT_TEST -include CppUTest/MemoryLeakDetectorNewMacros.h
COMPONENT_UT_LDFLAGS := $(LDFLAGS) -lstdc++ 

ALL_DEP += $(ALL_TST_OBJS:.obj=.d)
ALL_DEP += $(ALL_STUB_OBJS:.obj=.d)
endif

ifneq "$(MAKECMDGOALS)" "clean"
   -include $(ALL_DEP)
endif

ALL_CFLAGS := $(ALL_INCLUDE) $(COMPONENT_CFLAGS)
ALL_CXXFLAGS := $(ALL_INCLUDE) $(COMPONENT_CXXFLAGS)

$(OBJ_ROOT)/%.obj: $(COMPONENT_ROOT)/%.cpp
	@echo compiling $<
	@mkdir -p $(dir $@)
	$(CC) $(ALL_CXXFLAGS) -M -MT "$@ $(subst .obj,.d,$@)" $<
	@$(COMPILE.cpp) $(ALL_CXXFLAGS) $(OUTPUT_OPTION) $<

$(OBJ_ROOT)/%.obj: $(COMPONENT_ROOT)/%.c
	@echo compiling $<
	@mkdir -p $(dir $@)
	@$(CC) $(ALL_CFLAGS) -M -MT "$@ $(subst .obj,.d,$@)" $<
	@$(COMPILE.c) $(ALL_CFLAGS) $(OUTPUT_OPTION) $<	

$(COMPONENT_SRC_LIB): $(ALL_SRC_OBJS)
	@$(AR) -r $@ $^

$(COMPONENT_STUB_LIB): $(ALL_STUB_OBJS)
	@$(AR) -r $@ $^

$(COMPONENT_UT): $(ALL_TST_OBJS) $(COMPONENT_SRC_LIB) $(COMPONENT_STUB_LIB) $(CPPUTEST_LIB)
	@echo 'Building target: $@'
	$(CC) -u _main $^ $(COMPONENT_UT_LDFLAGS) -o $@

$(COMPONENT_PRODUCT): $(ALL_SRC_OBJS) $(COMPONENT_PRODUCT_LIBS)
	@echo 'Building target: $@'
	$(CC) -o $@ -g $^ $(COMPONENT_PRODUCT_LDFLAGS)
