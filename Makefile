### Compiler
# (override for cross-compilation, e.g. CC=riscv64-linux-gnu-gcc)
CC ?= gcc
LD ?= ld

# Directories
SOURCE_DIR = source

# Build options:
#   make                   SDL2 (default)
#   make SDL=1             SDL 1.2 (SDL1)
#   make SDL=1 FB=1        SDL1 + prefer framebuffer (fbcon)   [Linux only]
#   make FB=1              SDL2 + prefer framebuffer (KMSDRM)   [Linux only]
#
# Windows (MinGW) examples:
#   make                   SDL2
#   make SDL=1             SDL 1.2
#
# Cross example (Linux):
#   CC=riscv64-linux-gnu-gcc make SDL=1 FB=1

SDL ?= 2
FB ?= 0

# -DLEVELGENTEST
# -DTEST_SHOWPORTALPOS
# -DTEST_INVENTORY
# -DGODMODE

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    # ===================== Windows (MinGW) =====================
    OUTPUT  = game.exe
    CFLAGS += -Wall -Wextra -O2 -static -static-libgcc

    ifeq ($(SDL),1)
        # SDL 1.2 (SDL1) on Windows (MSYS2 MINGW32)
        # Package: mingw-w64-i686-SDL
        #
        # The static libSDL.a was built with DirectX 5/6/7 support.
        # It references many COM GUIDs that are defined in:
        #   - dxguid  (IID_*, GUID_*)
        #   - ddraw   (some DirectDraw symbols)
        #
        # You MUST add -ldxguid -lddraw right after -lSDL.
        CFLAGS += -DUSE_SDL1
        LDFLAGS += -lmingw32 -lSDLmain -lSDL -ldxguid -lddraw -ldinput8 -lm \
                   -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 \
                   -lole32 -loleaut32 -lversion -luuid -ladvapi32 \
                   -lsetupapi -lshell32
    else
        # SDL2 (default)
        CFLAGS += -DUSE_SDL2
        LDFLAGS += -lmingw32 -lSDL2main -lSDL2 -lm \
                   -lkernel32 -luser32 -lgdi32 -lwinmm -limm32 \
                   -lole32 -loleaut32 -lversion -luuid -ladvapi32 \
                   -lsetupapi -lshell32 -ldinput8
    endif

else
    # ===================== Linux / Unix =====================
    OUTPUT  = game

    ifeq ($(SDL),1)
        # SDL 1.2
        SDL_CFLAGS := $(shell sdl-config --cflags 2>/dev/null || echo "")
        SDL_LIBS   := $(shell sdl-config --libs 2>/dev/null || echo "-lSDL")
        CFLAGS += -DUSE_SDL1 $(SDL_CFLAGS)
        LDFLAGS += $(SDL_LIBS) -lm
    else
        # SDL 2 (default)
        CFLAGS += -DUSE_SDL2
        LDFLAGS += -lSDL2 -lm
    endif

    # Ensure one of the defines is always present (Linux)
    ifneq ($(filter -DUSE_SDL%,$(CFLAGS)),)
    else
        CFLAGS += -DUSE_SDL2
    endif

    # Framebuffer hints (Linux only)
    ifeq ($(FB),1)
        CFLAGS += -DUSE_FB
    endif
endif

# Common CFLAGS
CFLAGS += -Wall -Wextra -O2

# C Source files
SOURCES = $(wildcard 				  \
	$(SOURCE_DIR)/crafting/*.c 	      \
	$(SOURCE_DIR)/entity/*.c 	      \
	$(SOURCE_DIR)/entity/particle/*.c \
	$(SOURCE_DIR)/gfx/*.c 			  \
	$(SOURCE_DIR)/item/*.c 			  \
	$(SOURCE_DIR)/item/resource/*.c   \
	$(SOURCE_DIR)/level/*.c 		  \
	$(SOURCE_DIR)/level/levelgen/*.c  \
	$(SOURCE_DIR)/level/tile/*.c      \
	$(SOURCE_DIR)/screen/*.c          \
	$(SOURCE_DIR)/utils/*.c           \
	$(SOURCE_DIR)/*.c                 \
)

# C Header files
HEADERS = $(wildcard                  \
	$(SOURCE_DIR)/crafting/*.h        \
	$(SOURCE_DIR)/entity/*.h          \
	$(SOURCE_DIR)/entity/particle/*.h \
	$(SOURCE_DIR)/gfx/*.h             \
	$(SOURCE_DIR)/item/*.h            \
	$(SOURCE_DIR)/item/resource/*.h   \
	$(SOURCE_DIR)/level/*.h           \
	$(SOURCE_DIR)/level/levelgen/*.h  \
	$(SOURCE_DIR)/level/tile/*.h      \
	$(SOURCE_DIR)/screen/*.h          \
	$(SOURCE_DIR)/utils/*.h           \
	$(SOURCE_DIR)/*.h                 \
)

# Convert all .c files into .o files
OBJECTS = $(SOURCES:.c=.o)


# Default target: build the final executable
all: $(OUTPUT)


# Link all object files to create the executable
$(OUTPUT): $(OBJECTS)
	@$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS) -o $@


# Compile each .c file into a .o file
%.o: %.c $(HEADERS)
	@$(CC) $(CFLAGS) -c $< -o $@


# Run the game
run: $(OUTPUT)
	@./$(OUTPUT)


# Clean up build artifacts
clean:
	@rm -fv $(OBJECTS) $(OUTPUT)
