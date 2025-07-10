CC=x86_64-w64-mingw32-gcc
CC32=i686-w64-mingw32-gcc
WINDRES=x86_64-w64-mingw32-windres
STRIP=x86_64-w64-mingw32-strip
UPX=upx


CFLAGS=-Os -s -static -Wall -Wextra -DWIN32_LEAN_AND_MEAN \
       -ffunction-sections -fdata-sections -fno-ident -fomit-frame-pointer \
       -march=x86-64 -mtune=generic


LDFLAGS=-Wl,--gc-sections -Wl,--strip-all -Wl,--build-id=none \
        -static-libgcc -static-libstdc++ -Wl,--no-insert-timestamp


LIBS=-lwininet -lws2_32 -liphlpapi -luser32 -lkernel32 -lgdi32 \
     -lshell32 -ladvapi32 -lole32 -loleaut32 -luuid


TARGET=bl00dym4ry.exe
TARGET32=bl00dym4ry32.exe
SOURCE=main.c


$(TARGET): $(SOURCE)
	@echo "=== Building x64 Version ==="
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS) $(LIBS)
	@echo "=== Optimize.. ==="
	$(STRIP) --strip-all $(TARGET)
	@if command -v $(UPX) >/dev/null 2>&1; then \
		echo "=== Packed in UPX ==="; \
		$(UPX) --best --lzma $(TARGET) 2>/dev/null || echo "UPX Packed crash."; \
	else \
		echo "UPX not find, run as unpacked; \
	fi
	@echo "=== Complete ==="
	@echo "Size of file: $$(du -h $(TARGET) | cut -f1)"
	@file $(TARGET)

$(TARGET32): $(SOURCE)
	@echo "=== Building x86 Version ==="
	$(CC32) $(CFLAGS) -o $(TARGET32) $(SOURCE) $(LDFLAGS) $(LIBS)
	@echo "=== Optimize ==="
	i686-w64-mingw32-strip --strip-all $(TARGET32)
	@if command -v $(UPX) >/dev/null 2>&1; then \
		echo "=== Packed UPX ==="; \
		$(UPX) --best --lzma $(TARGET32) 2>/dev/null || echo "UPX Packed crash"; \
	else \
		echo "UPX not find, run as unpacked"; \
	fi
	@echo "=== Complete ==="
	@echo "Size of file: $$(du -h $(TARGET32) | cut -f1)"
	@file $(TARGET32)

all: $(TARGET) $(TARGET32)

debug: $(SOURCE)
	@echo "=== Building Debug Version ==="
	$(CC) -g -O0 -DDEBUG -o $(TARGET) $(SOURCE) $(LIBS)
	@echo "Complete"

clean:
	rm -f $(TARGET) $(TARGET32)
	@echo "clean Ok!"

check-deps:
	@echo "=== Check Deps  ==="
	@command -v $(CC) >/dev/null 2>&1 || (echo "MinGW-w64 not find! Setup: sudo pacman -S mingw-w64-gcc" && exit 1)
	@command -v $(UPX) >/dev/null 2>&1 || echo "UPX not find! Setup: sudo pacman -S upx"
	@command -v wine >/dev/null 2>&1 || echo "Wine not find! Setup: sudo pacman -S wine"
	@echo "Check Ok!"

install-deps:
	@echo "=== Install Deps ==="
	sudo pacman -S --needed mingw-w64-gcc mingw-w64-binutils mingw-w64-headers mingw-w64-crt upx wine
	@echo "Deps Ok!"


test: $(TARGET)
	@echo "=== Test in Wine ==="
	@if command -v wine >/dev/null 2>&1; then \
		wine $(TARGET); \
	else \
		echo "Wine not find! Setup: sudo pacman -S wine"; \
	fi

info: $(TARGET)
	@echo "=== Information ==="
	@ls -lh $(TARGET)
	@file $(TARGET)
	@echo "=== Sections ==="
	@x86_64-w64-mingw32-objdump -h $(TARGET)

help:
	@echo "Start command:"
	@echo "  make              - Building x64 Version"
	@echo "  make $(TARGET32)  - Building x86 Version"
	@echo "  make all          - Building both"
	@echo "  make debug        - Building Debug Version"
	@echo "  make clean        - Cleaning"
	@echo "  make check-deps   - Check Deps"
	@echo "  make install-deps - Setup Deps"
	@echo "  make test         - Test in Wine"
	@echo "  make info         - Information"
	@echo "  make help         - This Help"

.PHONY: all debug clean check-deps install-deps test info help
