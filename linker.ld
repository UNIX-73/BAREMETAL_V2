/* Linker script para Raspberry Pi Zero 2 W con soporte para vtables en Rust y secciones personalizadas */

ENTRY(_start);

MEMORY
{
    /* Ajusta estas direcciones y tamaños según tu hardware */
    FLASH (rx)  : ORIGIN = 0x8000, LENGTH = 16M
    RAM   (rwx) : ORIGIN = 0x40000000, LENGTH = 512M
}

/*
  Se agrupan en FLASH el código de arranque, el código principal y los datos de solo lectura
  (incluyendo las secciones que contienen las vtables y datos relocados).
*/
SECTIONS
{
    .flash_image :
    {
        /* Sección de arranque */
        _stext_boot = .;
        KEEP(*(.text.boot))
        _etext_boot = .;

        /* Código principal */
        _stext = .;
        KEEP(*(.text*))
        _etext = .;

        /* Datos de solo lectura (incluyendo vtables y secciones 'relocated') */
        _srodata = .;
        KEEP(*(.rodata*))
        KEEP(*(.vtable*))
        KEEP(*(.data.rel.ro*))
        KEEP(*(.sdata.rel.ro*))
        _erodata = .;
    } > FLASH

    /*
      La sección .data se ubicará en RAM y se carga desde FLASH justo después de .flash_image.
    */
    .data : AT(ADDR(.flash_image) + SIZEOF(.flash_image))
    {
        _sdata = .;
        KEEP(*(.data*))
        KEEP(*(.sdata*))
        _edata = .;
    } > RAM

    /*
      La sección .bss en RAM (NOLOAD) para datos no inicializados.
      Se definen los símbolos bss_begin y bss_end para usarlos en el código de arranque.
    */
    . = ALIGN(8);
    _sbss = .;
    .bss (NOLOAD) :
    {
        *(.bss*)
        *(COMMON)
    }
    _ebss = .;
    /* Definimos bss_begin y bss_end para que tengan los valores en RAM */
    PROVIDE(bss_begin = _sbss);
    PROVIDE(bss_end   = _ebss);

    /*
      Sección extra para id_pgd: se alinea a 0x1000 y se reserva 6 páginas (6 * 4096 bytes).
    */
    . = ALIGN(0x1000);
    id_pgd = .;
    .data.id_pgd :
    {
        . += (6 * (1 << 12));
    } > RAM
}
