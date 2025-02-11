# ================================================================
# Configuración y Variables
# ================================================================
SRC_DIR      := src
INCLUDE_DIR  := include
BUILD_DIR    := build
OBJ_DIR      := $(BUILD_DIR)/obj

ARMGNU       := aarch64-none-elf

# Opciones de compilación
CFLAGS       := -Wall -nostdlib -nostartfiles -ffreestanding $(addprefix -I, $(SRC_DIR) $(INCLUDE_DIR)) -mgeneral-regs-only
CPPFLAGS     := -Wall -nostdlib -nostartfiles -ffreestanding $(addprefix -I, $(SRC_DIR) $(INCLUDE_DIR)) -mgeneral-regs-only
ASMFLAGS     := $(addprefix -I, $(INCLUDE_DIR))

# ================================================================
# Recopilación de archivos fuente (buscando recursivamente)
# ================================================================
C_SRC    := $(shell find $(SRC_DIR) -type f -name '*.c')
CPP_SRC  := $(shell find $(SRC_DIR) -type f -name '*.cpp')
ASM_SRC  := $(shell find $(SRC_DIR) -type f -name '*.S')

OBJ_C    := $(patsubst $(SRC_DIR)/%.c,   $(OBJ_DIR)/%_c.o,   $(C_SRC))
OBJ_CPP  := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%_cpp.o, $(CPP_SRC))
OBJ_ASM  := $(patsubst $(SRC_DIR)/%.S,   $(OBJ_DIR)/%_s.o,   $(ASM_SRC))
OBJECTS  := $(OBJ_C) $(OBJ_CPP) $(OBJ_ASM)

# Advertencia si no se encuentran fuentes
ifeq ($(strip $(OBJECTS)),)
   $(warning No se encontraron archivos fuente en $(SRC_DIR))
endif

# ================================================================
# Objetivos principales
# ================================================================

# Objetivo por defecto: generar la imagen final
all: $(BUILD_DIR)/kernel8.img

# ------------------------------------------------
# Compilación de archivos fuente
# ------------------------------------------------

# Regla para compilar archivos C
$(OBJ_DIR)/%_c.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@echo "Compilando C: $<"
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

# Regla para compilar archivos C++
$(OBJ_DIR)/%_cpp.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@echo "Compilando C++: $<"
	$(ARMGNU)-g++ $(CPPFLAGS) -c $< -o $@

# Regla para compilar archivos Assembly
$(OBJ_DIR)/%_s.o: $(SRC_DIR)/%.S
	@mkdir -p $(@D)
	@echo "Compilando ASM: $<"
	$(ARMGNU)-gcc $(ASMFLAGS) -c $< -o $@

# ------------------------------------------------
# Enlace y generación de la imagen final
# ------------------------------------------------

# Enlaza todos los objetos para generar el binario ELF.
$(BUILD_DIR)/kernel8.elf: $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	@echo "Enlazando kernel8.elf..."
	$(ARMGNU)-ld -T linker.ld -o $@ $^

# A partir del ELF, genera la imagen binaria (kernel8.img)
$(BUILD_DIR)/kernel8.img: $(BUILD_DIR)/kernel8.elf
	@echo "Generando kernel8.img..."
	$(ARMGNU)-objcopy $< -O binary $@

# ================================================================
# Otros objetivos
# ================================================================
.PHONY: all clean

clean:
	@echo "Limpiando el directorio build..."
	rm -rf $(BUILD_DIR)
