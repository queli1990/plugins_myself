
ARCH ?= arm
EMPTY	=
SPACE	= $(EMPTY) $(EMPTY)
OUT = out/android/$(ARCH)

ifeq ($(ARCH),x86)
CXX = tools/android-toolchain-x86/bin/i686-linux-android-c++
else
CXX = tools/android-toolchain-arm/bin/arm-linux-androideabi-g++
endif

CXXFLAGS 	= -Wall -ffunction-sections -fdata-sections -O3 -std=c++0x -fexceptions -frtti \
            -Idepe/tinyxml2 -Idepe/rapidjson/include 
SOURCES = util.cc depe/tinyxml2/tinyxml2.cpp http.cc log.cc discovery.cc service.cc \
					dial.cc media-renderer.cc android.cc
OBJECTS = $(addsuffix .o,$(basename $(SOURCES)))

.PHONY: all out

all: $(OBJECTS)
	$(CXX) -shared -Wl,-soname=libdialclient.so -s -o $(OUT)/libdialclient.so \
	-Wl,--whole-archive -Wl,--gc-sections $(addprefix $(OUT)/,$(OBJECTS)) -Wl,--no-whole-archive \
	-llog -landroid

$(OBJECTS): out
	@mkdir -p $(dir $(OUT)/$@)
	$(CXX) $(CXXFLAGS) -MMD -MF $(OUT)/$@.d.raw -c -o $(OUT)/$@ $(basename $@).c*

out:
	mkdir -p $(OUT)