#pluma_PATH=$(HOME)/qmodule/pluma-2.4.2
include ../version
pluma_PATH=../packages/pluma-2.4.2

CXX := env LD_LIBRARY_PATH=$(GCC_PATH)/lib:$(MPC_PATH)/lib:$(MPFR_PATH)/lib:$(GMP_PATH)/lib $(GCC_PATH)/bin/g++

CXXFLAGS := -g3 -O0 -std=c++11 \
			-fno-strict-aliasing -Wall -Wno-error=unused-but-set-variable \
			-Wno-error=unused-but-set-parameter -Werror

CPPFLAGS := -I$(pluma_PATH)/include

LDFLAGS := -L$(pluma_PATH)/lib

RTFLAGS := -Wl,-rpath=$(pluma_PATH)/lib -Wl,-rpath=/$(GCC_PATH)/lib64

LIBS := -lpluma

SRC := $(wildcard *.cc) $(wildcard interface/*.cc)
OBJ := $(patsubst %.cc, %.o, $(SRC))
DEP := $(patsubst %.o, %.d, $(OBJ))

ifeq ($(USE_DEP),1)
-include $(DEP)
endif

TARGET := sort

all: 
	$(MAKE) USE_DEP=1 target

target: $(TARGET)

sort: $(OBJ) 
	$(CXX) $^ -o $@ $(RTFLAGS) $(LDFLAGS) $(LIBS)

%.o : %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%.d : %.cc 
	@$(CXX) -MM $< $(CPPFLAGS) $(CXXFLAGS) | sed 's/$(notdir $*)\.o/$(subst /,\/,$*).o $(subst /,\/,$*).d/g' > $@

clean:
	-rm -rf $(OBJ) $(TARGET) $(DEP)

.PHONY: all target clean

