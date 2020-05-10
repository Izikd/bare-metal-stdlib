# SPDX-License-Identifier: MIT

# Arch
ARCH ?=

ifeq ($(ARCH),)
$(error ARCH must be set)
endif

# Directories
DEFAULT_APP := hello_world
BUILD_DIR := build
COMMON_DIR := common
APPS_DIR := apps
ARCH_DIR := arch/$(ARCH)

ifeq ($(wildcard $(ARCH_DIR)),)
$(error Unkown ARCH=$(ARCH))
endif

# Include arch Makefile with flags
include $(ARCH_DIR)/Makefile

# Flags
AFLAGS := \
	$(ARCH_AFLAGS) \
	-D__ASSEMBLY__ \
	-g \
	-MD -MP \

CFLAGS := \
	$(ARCH_CFLAGS) \
	-Wall -Werror \
	-g \
	-MD -MP \

# Enable extra warning if W=1 is passed
ifeq ($(W),1)
CFLAGS += \
	-Wextra -Wpedantic
else
CFLAGS += \
	-Wno-unused-function
endif

LDFLAGS := \
	$(ARCH_LDFLAGS) \

ARCH_LINKER_SCRIPT ?=

# Print executed commands if V=1 is passed
ifeq ($(V),1)
Q :=
else
Q := @
endif

# Functions
define target_name
$(Q)printf "% 5s %s\n" $(1) $(2)
endef

define find_objs
$(foreach suffix,c S,$(patsubst %.$(suffix),$(BUILD_DIR)/%.o,$(shell find $(1) -not -type d -name '*.$(suffix)')))
endef

define include_path
$(patsubst %,-I%,$(shell find $(1) -mindepth $(2) -mindepth $(3) -type d))
endef

# Applications
APPS_DIRS := $(wildcard $(APPS_DIR)/*)
APPS_NAMES := $(patsubst $(APPS_DIR)/%,%,$(APPS_DIRS))
APPS_TARGETS := $(addprefix $(BUILD_DIR)/,$(APPS_NAMES))
APPS_EASY_TARGETS := $(addprefix app_,$(APPS_NAMES))

APPS_SRCS = $(foreach dir,$(APPS_DIRS),$(wildcard $(dir)/*.[cS]))

# Run
RUN_TARGETS := $(addprefix run_,$(APPS_NAMES))

# Objects
COMMON_OBJS := $(call find_objs,$(COMMON_DIR))
ARCH_OBJS := $(call find_objs,$(ARCH_DIR))
APPS_OBJS := $(call find_objs,$(APPS_DIRS))

# Add include path
INCLUDE_PATH := \
	$(call include_path,$(COMMON_DIR),1,1) \
	$(call include_path,$(ARCH_DIR),1,1) \

AFLAGS += $(INCLUDE_PATH)
CFLAGS += $(INCLUDE_PATH)

# Phony targets
all: app_$(DEFAULT_APP)

$(APPS_EASY_TARGETS): app_%: $(BUILD_DIR)/%

$(RUN_TARGETS): run_%: $(BUILD_DIR)/%
	$(call target_name,RUN,$<)
	$(Q)$(ARCH_RUN)

clean:
	$(call target_name,RM,$(BUILD_DIR))
	$(Q)rm -rf $(BUILD_DIR)

help:
	$(Q)echo "Supported ARCH: $(patsubst arch/%,%,$(wildcard arch/*))"

print_debug:
	$(Q)echo "APPS_DIRS = $(APPS_DIRS)"
	$(Q)echo "APPS_NAMES = $(APPS_NAMES)"
	$(Q)echo "APPS_TARGETS = $(APPS_TARGETS)"
	$(Q)echo "COMMON_OBJS = $(COMMON_OBJS)"
	$(Q)echo "ARCH_OBJS = $(ARCH_OBJS)"
	$(Q)echo "APPS_OBJS = $(APPS_OBJS)"
	$(Q)echo "INCLUDE_PATH = $(INCLUDE_PATH)"

.PHONY: all clean print_debug $(APPS_EASY_TARGETS) $(RUN_TARGETS) FORCE

# Targets per file type

# Force rebuild if F=1 is passed
ifeq ($(F),1)
FORCE := FORCE
else
FORCE :=
endif

$(BUILD_DIR)/config.log: FORCE
	$(call target_name,GEN,$@)
	$(Q)mkdir -p $(@D)
	$(Q)echo "# Config check file" > $@.tmp
	$(Q)echo >> $@.tmp
	$(Q)echo "CROSS_COMPILE = $(CROSS_COMPILE)" >> $@.tmp
	$(Q)echo "ARCH = $(ARCH)" >> $@.tmp
	$(Q)echo "CFLAGS = $(CFLAGS)" >> $@.tmp
	$(Q)echo "AFLAGS = $(AFLAGS)" >> $@.tmp
	$(Q)echo "LDFLAGS = $(LDFLAGS)" >> $@.tmp
	$(Q)diff $@.tmp $@ >/dev/null 2>&1 || mv $@.tmp $@
	$(Q)rm -f $@.tmp

$(BUILD_DIR)/%.o: %.S $(BUILD_DIR)/config.log $(FORCE)
	$(call target_name,AS,$@)
	$(Q)mkdir -p $(@D)
	$(Q)$(AS) $(AFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/config.log $(FORCE)
	$(call target_name,CC,$@)
	$(Q)mkdir -p $(@D)
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

define create_app_target
APP_OBJS_$(1) := $(filter $(BUILD_DIR)/$(APPS_DIR)/$(1)/%,$(APPS_OBJS))

$(BUILD_DIR)/$(1): $(COMMON_OBJS) $(ARCH_OBJS) $$(APP_OBJS_$(1)) $(ARCH_LINKER_SCRIPT) $(BUILD_DIR)/config.log $(FORCE)
	$(call target_name,LD,$$@)
	$(Q)$(LD) $(COMMON_OBJS) $(ARCH_OBJS) $$(APP_OBJS_$(1)) $(LDFLAGS) -o $$@
endef

$(foreach app,$(APPS_NAMES),$(eval $(call create_app_target,$(app))))

# Include '.d' files for GCC generated dependency files
-include $(patsubst %.o,%.d,$(ARCH_OBJS) $(APPS_OBJS))
