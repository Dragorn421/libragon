ROMNAME := envargs

BUILD_DIR = build

LIBDRAGON_PREVIEW = 2
include $(N64_INST)/include/n64.mk

all: $(ROMNAME).z64
.PHONY: all

OBJS := $(BUILD_DIR)/envargs.o

$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

$(ROMNAME).z64: N64_ROM_TITLE = "env & args"

clean:
	$(RM) -r $(BUILD_DIR) *.z64
.PHONY: clean

ifneq ($(wildcard $(BUILD_DIR)),)
  -include $(shell find $(BUILD_DIR) -name '*.d')
endif
