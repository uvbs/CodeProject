include ../version

PLATFORM := $(shell uname)
CPUS := $(strip $(if $(shell echo $(PLATFORM)|grep Linux),\
		$(shell cat /proc/cpuinfo|grep -c processor),\
		$(shell sysctl -a | egrep -i 'hw.ncpu' | cut -d: -f2)))

CXX := env LD_LIBRARY_PATH=$(GCC_PATH)/lib:$(MPC_PATH)/lib:$(MPFR_PATH)/lib:$(GMP_PATH)/lib $(GCC_PATH)/bin/g++

CXXFLAGS := -DNDEBUG -g3 -O2 -fPIC -std=c++11 \
			-fno-strict-aliasing -Wall -Wno-error=unused-but-set-variable \
			-Wno-error=unused-but-set-parameter -Werror

CPPFLAGS := -I../include

LDFLAGS := 
RTFLAGS := 
LIBS := -ldl

SRC := $(wildcard *.cc)
OBJ := $(patsubst %.cc, %.o, $(SRC))
DEP := $(patsubst %.o, %.d, $(OBJ))

SHARED_LIB = lib$(LIB_NAME).so
STATIC_LIB = lib$(LIB_NAME).a

LIB_VERSION_MAJOR= $(shell echo $(LIB_VERSION) | cut -d. -f1)

all:
	$(MAKE) -j$(CPUS) USE_DEP=1 target

target: shared static

shared: $(SHARED_LIB) $(SHARED_LIB).$(LIB_VERSION) $(SHARED_LIB).$(LIB_VERSION_MAJOR)

static: $(STATIC_LIB)

$(SHARED_LIB).$(LIB_VERSION): $(OBJ)
	$(CXX) $^ -o $@ $(RTFLAGS) $(LDFLAGS) $(LIBS) -shared -Wl,-soname,$(SHARED_LIB).$(LIB_VERSION_MAJOR)

$(SHARED_LIB).$(LIB_VERSION_MAJOR): $(SHARED_LIB).$(LIB_VERSION)
	ln -sf $^ $@

$(SHARED_LIB): $(SHARED_LIB).$(LIB_VERSION_MAJOR)
	ln -sf $^ $@

$(STATIC_LIB): $(OBJ) 
	ar rcs $@ $^

ifeq ($(USE_DEP), 1)
-include $(DEP)
endif

%.o : %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

%.d : %.cc
	@$(CXX) -MM $< $(CPPFLAGS) $(CXXFLAGS) | sed 's/$(notdir $*)\.o/$(subst /,\/,$*).o $(subst /,\/,$*).d/g' > $@

clean:
	-rm -rf $(OBJ) $(DEP) *.a *.so *.so.*

install: all
	/usr/bin/install -m 664 -c $(STATIC_LIB) $(PACKAGE_PATH)/lib/
	/usr/bin/install -c $(SHARED_LIB).$(LIB_VERSION) $(PACKAGE_PATH)/lib/
	/sbin/ldconfig -n $(PACKAGE_PATH)/lib
	(cd $(PACKAGE_PATH)/lib; ln -s $(SHARED_LIB).$(LIB_VERSION_MAJOR) $(SHARED_LIB))

.PHONY: all target clean shared static install

