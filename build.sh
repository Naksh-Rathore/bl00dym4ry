#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' 

log() {
    echo -e "${GREEN}[$(date '+%Y-%m-%d %H:%M:%S')]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
    exit 1
}

warn() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}


check_dependencies() {
    log "Check Deps..."
    
    if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
        error "MinGW-w64 not find! Setup: sudo pacman -S mingw-w64-gcc"
    fi
    
    if ! command -v upx &> /dev/null; then
        warn "UPX not find! Setup: sudo pacman -S upx"
    fi
    
    log "Deps OK!"
}

compile() {
    local arch=$1
    local output=$2
    local compiler=$3
    
    log "Building $arch Version..."
    
    $compiler \
        -Os -s -static \
        -ffunction-sections -fdata-sections -fno-ident -fomit-frame-pointer \
        -march=x86-64 -mtune=generic \
        -o $output main.c \
        -Wl,--gc-sections -Wl,--strip-all -Wl,--build-id=none \
        -static-libgcc -static-libstdc++ \
        -lwininet -lws2_32 -liphlpapi -luser32 -lkernel32 -lgdi32 \
        -lshell32 -ladvapi32 -lole32 -loleaut32 -luuid
    
    if [ $? -eq 0 ]; then
        log "Building $arch Complete!"
    else
        error "Error $arch"
    fi
}

optimize() {
    local file=$1
    local arch=$2
    
    log "Packing $arch Version..."
    
    if [ "$arch" == "64-bit" ]; then
        x86_64-w64-mingw32-strip --strip-all $file
    else
        i686-w64-mingw32-strip --strip-all $file
    fi
    
    if command -v upx &> /dev/null; then
        log "Packing UPX..."
        upx --best --lzma $file 2>/dev/null || warn "UPX packing not complete!"
    fi
    
    local size=$(du -h $file | cut -f1)
    log "Size of File $file: $size"
}


main() {
    log "Building Bl00dyM4ry..."
    
    check_dependencies
    
    if [ ! -f "main.c" ]; then
        error "main.c not found!"
    fi
    
    compile "64-bit" "bl00dymary.exe" "x86_64-w64-mingw32-gcc"
    optimize "bl00dymary.exe" "64-bit"
    
    if [ "$1" == "all" ]; then
        compile "32-bit" "bl00dymary32.exe" "i686-w64-mingw32-gcc"
        optimize "bl00dymary32.exe" "32-bit"
    fi
    
    log "Build Complete!"
    
    echo -e "\n${BLUE}=== TESTING COMPILATION ===${NC}"
    ls -lah bl00dymary*.exe 2>/dev/null || echo "No building files"
    
    echo -e "\n${BLUE}=== INFORMATION ===${NC}"
    file bl00dymary*.exe 2>/dev/null || echo "File not Found"
}

main $@
