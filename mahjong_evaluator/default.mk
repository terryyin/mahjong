.PHONY: all clean
all: $(COMPONENT_UT)
	./$^
	
clean:
	-$(RM) $(COMPONENT_UT) $(COMPONENT_SRC_LIB) $(COMPONENT_STUB_LIB)
	-$(RM) $(COMPONENT_DEP)
	-$(RM) $(COMPONENT_ALL_SRC_OBJS) $(COMPONENT_ALL_TST_OBJS) $(COMPONENT_ALL_STUB_OBJS)

COMPONENT_ALL_SRC_DIRS := $(foreach x,$(COMPONENT_SRC_DIRS),$(COMPONENT_ROOT)/$x)
COMPONENT_ALL_TST_DIRS := $(foreach x,$(COMPONENT_TST_DIRS),$(COMPONENT_ROOT)/$x)
COMPONENT_ALL_STUB_DIRS:= $(foreach x,$(COMPONENT_STUB_DIRS),$(COMPONENT_ROOT)/$x)

get_src = $(wildcard $1/*.cpp) $(wildcard $1/*.c)
COMPONENT_ALL_SRC := $(foreach x,$(COMPONENT_ALL_SRC_DIRS),$(call get_src,$x))
COMPONENT_ALL_TST := $(foreach x,$(COMPONENT_ALL_TST_DIRS),$(call get_src,$x))
COMPONENT_ALL_STUB:= $(foreach x,$(COMPONENT_ALL_STUB_DIRS),$(call get_src,$x))
src_to_obj = $(subst .c,.obj,$(subst .cpp,.obj,$1))
COMPONENT_ALL_SRC_OBJS := $(foreach f,$(COMPONENT_ALL_SRC),$(call src_to_obj,$f))
COMPONENT_ALL_TST_OBJS := $(foreach f,$(COMPONENT_ALL_TST),$(call src_to_obj,$f))
COMPONENT_ALL_STUB_OBJS:= $(foreach f,$(COMPONENT_ALL_STUB),$(call src_to_obj,$f))

COMPONENT_ALL_INCLUDES := $(foreach x,$(COMPONENT_ALL_SRC_DIRS),-I $x) \
                          $(foreach x,$(COMPONENT_ALL_TST_DIRS),-I $x) \
                          $(foreach x,$(COMPONENT_ALL_STUB_DIRS),-I $x) \
                          $(COMPONENT_INCLUDE)

CFLAGS := $(COMPONENT_CFLAGS) $(COMPONENT_ALL_INCLUDES)
CXXFLAGS := $(COMPONENT_CXXFLAGS) $(COMPONENT_ALL_INCLUDES)

ifneq "$(MAKECMDGOALS)" "product"
CFLAGS +=  -DUNIT_TEST  -fprofile-arcs -ftest-coverage
CXXFLAGS +=  -DUNIT_TEST  -fprofile-arcs -ftest-coverage
CPPUTEST_LIB := $(CPPUTEST_ROOT)/lib/libCppUTest.a $(CPPUTEST_ROOT)/lib/libCppUTestExt.a
CPPUTEST_INCLUDE = -I$(CPPUTEST_ROOT)/include
CFLAGS +=  $(CPPUTEST_INCLUDE) -include CppUTest/MemoryLeakDetectorMallocMacros.h
CXXFLAGS +=  $(CPPUTEST_INCLUDE) -include CppUTest/MemoryLeakDetectorNewMacros.h
LDFLAGS += -lstdc++ -fprofile-arcs -ftest-coverage

get_dep = $(subst .c,.d,$(subst .cpp,.d,$1))
COMPONENT_DEP := $(call get_dep,$(COMPONENT_ALL_SRC))
COMPONENT_DEP += $(call get_dep,$(COMPONENT_ALL_TST))
COMPONENT_DEP += $(call get_dep,$(COMPONENT_STUB))

ifneq "$(MAKECMDGOALS)" "clean"
   -include $(COMPONENT_DEP)
endif
endif

%.obj: %.cpp
	@echo compiling $<
	@$(CC) $(CXXFLAGS) -M -MF $(subst .obj,.d,$@) -MT "$@ $(subst .obj,.d,$@)" $<
	@$(COMPILE.cpp) $(CXXFLAGS) $(OUTPUT_OPTION) $<

%.obj: %.c
	@echo compiling $<
	@$(CC) $(CFLAGS) -M -MF $(subst .obj,.d,$@) -MT "$@ $(subst .obj,.d,$@)" $<
	@$(COMPILE.c) $(CFLAGS) $(OUTPUT_OPTION) $<	

$(COMPONENT_SRC_LIB): $(COMPONENT_ALL_SRC_OBJS)
	@$(AR) -r $@ $^
	
$(COMPONENT_STUB_LIB): $(COMPONENT_ALL_STUB_OBJS)
	@$(AR) -r $@ $^

$(COMPONENT_UT): $(COMPONENT_ALL_TST_OBJS) $(COMPONENT_SRC_LIB) $(COMPONENT_STUB_LIB) $(CPPUTEST_LIB)
	@echo 'Building target: $@ by $^'
	@$(CC) -u _main $^ $(LDFLAGS) -o $@

	