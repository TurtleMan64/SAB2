#TODO: setup a way to compile without -g when not debugging

TARGET_EXEC ?= a.out

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d) ../Libraries/Include/
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) `pkg-config --cflags glfw3 ogg vorbis vorbisfile openal gl`

CPPFLAGS ?= -g -Wall $(INC_FLAGS) -MMD -MP# -D_GLIBCXX_USE_CXX11_ABI=0
LDFLAGS := -lSOIL -lm -lpthread -ldl `pkg-config --static --libs glfw3 ogg vorbis vorbisfile openal gl`
CPPFLAGS += -D'fopen_s(pFile,filename,mode)=((*(pFile))=fopen((filename),(mode)))==NULL'

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

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
