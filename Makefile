TARGET_EXEC ?= yafm
DESTDIR= /bin/.
BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
CLIBS= -Wall -pedantic -g -lncurses -L/usr/local/musl/lib

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
CC= clang

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -Wall -pedantic -g -march=native

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(CLIBS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

install: $(BUILD_DIR)/$(TARGET_EXEC)
	cp ${BUILD_DIR}/${TARGET_EXEC} ${DESTDIR}

-include $(DEPS)

MKDIR_P ?= mkdir -p
