# ================================================================
# Configuración y Variables
# ================================================================
SRC_DIR      := src
INCLUDE_DIR  := include
BUILD_DIR    := build
OBJ_DIR      := $(BUILD_DIR)/obj
ELF_DIR      := $(BUILD_DIR)/elf
LIB_DIR      := $(BUILD_DIR)/lib

ARMGNU       := aarch64-none-elf

# Opciones de compilación para C, C++, ASM
CFLAGS       := -Wall -nostdlib -nostartfiles -ffreestanding $(addprefix -I, $(SRC_DIR) $(INCLUDE_DIR)) -mgeneral-regs-only
CPPFLAGS     := -Wall -nostdlib -nostartfiles -ffreestanding $(addprefix -I, $(SRC_DIR) $(INCLUDE_DIR)) -mgeneral-regs-only
ASMFLAGS     := $(addprefix -I, $(INCLUDE_DIR))

# ================================================================
# Fuentes y Objetos para C, C++ y ASM
# ================================================================
C_SRC    := $(shell find $(SRC_DIR) -type f -name '*.c')
CPP_SRC  := $(shell find $(SRC_DIR) -type f -name '*.cpp')
ASM_SRC  := $(shell find $(SRC_DIR) -type f -name '*.S')

OBJ_C    := $(patsubst $(SRC_DIR)/%.c,   $(OBJ_DIR)/%_c.o,   $(C_SRC))
OBJ_CPP  := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%_cpp.o, $(CPP_SRC))
OBJ_ASM  := $(patsubst $(SRC_DIR)/%.S,   $(OBJ_DIR)/%_s.o,   $(ASM_SRC))
OBJECTS  := $(OBJ_C) $(OBJ_CPP) $(OBJ_ASM)

ifeq ($(strip $(OBJECTS)),)
   $(warning No se encontraron archivos fuente C/C++/ASM en $(SRC_DIR))
endif

# ================================================================
# Configuración del crate Rust
# ================================================================
# Ahora el Cargo.toml se encuentra en la raíz y el código Rust inicia en src/rust_crate
RUST_CRATE_DIR := .
# Se espera que el crate se llame "baremetal_rust", generando libbaremetal_rust.a
RUST_LIB     := $(LIB_DIR)/libbaremetal_rust.a

# ================================================================
# Objetivos principales
# ================================================================
all: $(BUILD_DIR)/kernel8.img armstub

# ------------------------------------------------
# Compilación de archivos fuente en C, C++ y ASM
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

# ------------------------------------------------
# Compilación del crate Rust con Cargo
# ------------------------------------------------
$(RUST_LIB): Cargo.toml $(wildcard $(SRC_DIR)/rust_crate/*.rs)
	@mkdir -p $(LIB_DIR)
	@echo "Compilando crate Rust..."
	cargo build --release --target aarch64-unknown-none --lib
	@echo "Copiando la biblioteca Rust a $(RUST_LIB)..."
	cp target/aarch64-unknown-none/release/libbaremetal_rust.a $(RUST_LIB)

# ------------------------------------------------
# Enlace y generación del kernel final
# ------------------------------------------------
$(ELF_DIR)/kernel8.elf: $(OBJECTS) $(RUST_LIB)
	@mkdir -p $(ELF_DIR)
	@echo "Enlazando kernel8.elf..."
	$(ARMGNU)-ld -T linker.ld -o $@ $^

$(BUILD_DIR)/kernel8.img: $(ELF_DIR)/kernel8.elf
	@echo "Generando kernel8.img..."
	$(ARMGNU)-objcopy $< -O binary $@

# ================================================================
# armstub (compilación separada)
# ================================================================
armstub/build/armstub_s.o: armstub/src/armstub.S
	@mkdir -p $(dir $@)
	@echo "Compilando ARMSTUB ASM: $< -> $@"
	$(ARMGNU)-gcc $(ASMFLAGS) -c $< -o $@

armstub: armstub/build/armstub_s.o
	@mkdir -p $(ELF_DIR)
	@echo "Enlazando ARMSTUB..."
	$(ARMGNU)-ld -o $(ELF_DIR)/armstub.elf armstub/build/armstub_s.o
	@echo "Generando ARMSTUB binario..."
	$(ARMGNU)-objcopy $(ELF_DIR)/armstub.elf -O binary $(BUILD_DIR)/armstub-new.bin

# ================================================================
# Otros objetivos
# ================================================================
.PHONY: all clean armstub

clean:
	@echo "Limpiando el directorio build..."
	rm -rf $(BUILD_DIR)
	@echo "Limpiando el crate Rust (opcional)..."
	cargo clean
