# ================================================================
# Configuración y Variables
# ================================================================
SRC_DIR      := src
INCLUDE_DIR  := include
BUILD_DIR    := build
OBJ_DIR      := $(BUILD_DIR)/obj
ELF_DIR      := $(BUILD_DIR)/elf

ARMGNU       := aarch64-none-elf

# Opciones de compilación para C, C++, Rust y ASM
CFLAGS       := -Wall -nostdlib -nostartfiles -ffreestanding $(addprefix -I, $(SRC_DIR) $(INCLUDE_DIR)) -mgeneral-regs-only
CPPFLAGS     := -Wall -nostdlib -nostartfiles -ffreestanding $(addprefix -I, $(SRC_DIR) $(INCLUDE_DIR)) -mgeneral-regs-only
ASMFLAGS     := $(addprefix -I, $(INCLUDE_DIR))
RUSTFLAGS    := -C opt-level=3

# ================================================================
# Recopilación de archivos fuente (buscando recursivamente)
# ================================================================
C_SRC    := $(shell find $(SRC_DIR) -type f -name '*.c')
CPP_SRC  := $(shell find $(SRC_DIR) -type f -name '*.cpp')
ASM_SRC  := $(shell find $(SRC_DIR) -type f -name '*.S')
RUST_SRC := $(shell find $(SRC_DIR) -type f -name '*.rs')

OBJ_C    := $(patsubst $(SRC_DIR)/%.c,   $(OBJ_DIR)/%_c.o,   $(C_SRC))
OBJ_CPP  := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%_cpp.o, $(CPP_SRC))
OBJ_ASM  := $(patsubst $(SRC_DIR)/%.S,   $(OBJ_DIR)/%_s.o,   $(ASM_SRC))
OBJ_RUST := $(patsubst $(SRC_DIR)/%.rs,  $(OBJ_DIR)/%_rust.o, $(RUST_SRC))
OBJECTS  := $(OBJ_C) $(OBJ_CPP) $(OBJ_ASM) $(OBJ_RUST)

ifeq ($(strip $(OBJECTS)),)
   $(warning No se encontraron archivos fuente en $(SRC_DIR))
endif

# ================================================================
# Objetivos principales
# ================================================================
all: $(BUILD_DIR)/kernel8.img armstub

# ------------------------------------------------
# Compilación de archivos fuente
# ------------------------------------------------
$(OBJ_DIR)/%_c.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@echo "Compilando C: $<"
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%_cpp.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@echo "Compilando C++: $<"
	$(ARMGNU)-g++ $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%_s.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	@echo "Compilando ASM: $<"
	$(ARMGNU)-gcc $(ASMFLAGS) -c $< -o $@

$(OBJ_DIR)/%_rust.o: $(SRC_DIR)/%.rs
	@mkdir -p $(@D)
	@echo "Compilando Rust: $<"
	rustc --target=aarch64-unknown-none $(RUSTFLAGS) --crate-type=staticlib --emit=obj -o $@ $<

# ------------------------------------------------
# Enlace y generación del kernel final
# ------------------------------------------------

# Genera el ELF del kernel en build/elf
$(ELF_DIR)/kernel8.elf: $(OBJECTS)
	@mkdir -p $(ELF_DIR)
	@echo "Enlazando kernel8.elf..."
	$(ARMGNU)-ld -T linker.ld -o $@ $^

# A partir del ELF, genera la imagen binaria en build/
$(BUILD_DIR)/kernel8.img: $(ELF_DIR)/kernel8.elf
	@echo "Generando kernel8.img..."
	$(ARMGNU)-objcopy $< -O binary $@

# ================================================================
# Otros objetivos
# ================================================================
.PHONY: all clean

clean:
	@echo "Limpiando el directorio build..."
	rm -rf $(BUILD_DIR)/*

# ================================================================
# armstub (compilación separada)
# ================================================================
armstub/build/armstub_s.o: armstub/src/armstub.S
	@echo "Compilando ARMSTUB ASM: $< -> $@"
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(ASMFLAGS) -c $< -o $@

armstub: armstub/build/armstub_s.o
	@mkdir -p $(ELF_DIR)
	@echo "Enlazando ARMSTUB..."
	$(ARMGNU)-ld -o $(ELF_DIR)/armstub.elf armstub/build/armstub_s.o
	@echo "Generando ARMSTUB binario..."
	$(ARMGNU)-objcopy $(ELF_DIR)/armstub.elf -O binary $(BUILD_DIR)/armstub-new.bin
