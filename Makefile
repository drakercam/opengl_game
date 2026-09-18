BUILD_DIR := build
TARGET := untitled
TARGET_GPU := DRI_PRIME=1

CMAKE := cmake
GENERATOR := Ninja

.PHONY: all configure build run debug sanitize clean rebuild

all: build

configure:
	$(CMAKE) -S . -B $(BUILD_DIR) \
		-G $(GENERATOR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DENABLE_SANITIZERS=OFF

build:
	$(CMAKE) --build $(BUILD_DIR)

run: build
	cd $(BUILD_DIR) && $(TARGET_GPU) ./$(TARGET)

debug: build
	gdb ./$(BUILD_DIR)/$(TARGET)

sanitize:
	$(CMAKE) -S . -B $(BUILD_DIR) \
		-G $(GENERATOR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DENABLE_SANITIZERS=ON
	$(CMAKE) --build $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean configure build

#make configure    # first setup / change configuration
#make              # normal incremental build
#make run          # build + run
#make debug        # build + GDB
#make sanitize     # reconfigure with ASan/UBSan + build
#make rebuild      # wipe + configure + build
