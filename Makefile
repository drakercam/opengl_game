BUILD_DIR := build
WEB_BUILD_DIR := build-web

TARGET := untitled
TARGET_GPU := DRI_PRIME=1

CMAKE := cmake
GENERATOR := Ninja

.PHONY: all configure build run debug sanitize clean rebuild \
        web-configure web web-run web-clean web-rebuild


# =========================================================
# NATIVE
# =========================================================

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


# =========================================================
# WEB / EMSCRIPTEN
# =========================================================

web-configure:
	emcmake $(CMAKE) -S . -B $(WEB_BUILD_DIR) \
		-G $(GENERATOR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DENABLE_SANITIZERS=OFF

web:
	$(CMAKE) --build $(WEB_BUILD_DIR)

web-run: web
	emrun $(WEB_BUILD_DIR)/$(TARGET).html

web-clean:
	rm -rf $(WEB_BUILD_DIR)

web-rebuild: web-clean web-configure web


# =========================================================
# USAGE
# =========================================================

# make configure
# make
# make run
# make debug
# make sanitize
# make rebuild
#
# make web-configure
# make web
# make web-run
# make web-rebuild
# make web-clean
