UNAME_S := $(shell uname -s)

TARGET_EXEC ?= ../NewSonicThing/SAB2.out
BUILD_DIR ?= makebuild
SRC_DIRS ?= NewSonicThing/src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c)

ifeq ($(UNAME_S),Darwin)
	INC_FLAGS := $(shell pkg-config --cflags glfw3 ogg vorbis vorbisfile openal soil sdl2) -I./Libraries/Include/ # $(addprefix -I, $(shell find $(SRC_DIRS) -type d))
	CFLAGS := -O3 -Wall -Wextra -MMD -MP
	CPPFLAGS ?= $(INC_FLAGS)
	ALL_LDFLAGS := -sectcreate __TEXT __info_plist Info.plist -lm -lpthread -ldl `pkg-config --static --libs glfw3 ogg vorbis vorbisfile openal soil sdl2` -framework OpenGL $(LDFLAGS)
	CPPFLAGS += -D'fopen_s(pFile,filename,mode)=((*(pFile))=fopen((filename),(mode)))==NULL'
	CXXFLAGS ?= -O3 -Wall -Wextra -MMD -MP -std=c++17
else
	INC_FLAGS := `pkg-config --cflags glfw3 ogg vorbis vorbisfile openal gl` $(shell find $(SRC_DIRS) -type d) -I./Libraries/Include/
	CPPFLAGS ?= -O2 -Wall -Wextra $(INC_FLAGS) -MMD -MP # -D_GLIBCXX_USE_CXX11_ABI=0
	ALL_LDFLAGS := -lSOIL -lSDL2 -lm -lpthread -ldl `pkg-config --static --libs glfw3 ogg vorbis vorbisfile openal gl` $(LDFLAGS)
	CPPFLAGS += -D'fopen_s(pFile,filename,mode)=((*(pFile))=fopen((filename),(mode)))==NULL'
endif

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(ALL_LDFLAGS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
