/* Tell the linker that we want an x86_64 ELF64 output file */
OUTPUT_FORMAT(elf64-x86-64)

ENTRY(kernel_main)

/* Define the program headers we want so the bootloader gives us the right */
/* MMU permissions; this also allows us to exert more control over the linking */
/* process. */
PHDRS
{
  limine_requests PT_LOAD;
  text PT_LOAD;
  rodata PT_LOAD;
  data PT_LOAD;
}

SECTIONS
{
  /* Load the kernel into the higher half of virtual memory space. */
  /* The Limine bootloader loads the kernel at 0x100000 in the physical memory */
  . = 0xffffffff80000000;

  /* Define a section to contain the Limine requests and assign it to its own PHDR */
  .limine_requests : {
    KEEP(*(.limine_requests_start))
    KEEP(*(.limine_requests))
    KEEP(*(.limine_requests_end))
  } :limine_requests

  /* Move to the next memory page for .text */
  . = ALIGN(CONSTANT(MAXPAGESIZE));

  .text : {
    *(.text .text.*)
  } :text

  /* Move to the next memory page for .rodata */
  . = ALIGN(CONSTANT(MAXPAGESIZE));

  .rodata : {
    *(.rodata .rodata.*)
  } :rodata

  /* Add a .note.gnu.build-id output section in case a build ID flag is added to the */
  /* linker command. */
  .note.gnu.build-id : {
    *(.note.gnu.build-id)
  } :rodata

  /* Move to the next memory page for .data */
  . = ALIGN(CONSTANT(MAXPAGESIZE));

  .data : {
    *(.data .data.*)
  } :data

  /* NOTE: .bss needs to be the last thing mapped to :data, otherwise lots of */
  /* unnecessary zeros will be written to the binary. */
  /* If you need, for example, .init_array and .fini_array, those should be placed */
  /* above this. */
  .bss : {
    *(.bss .bss.*)
    *(COMMON)
  } :data

  /* Discard .note.* and .eh_frame* since they may cause issues on some hosts. */
  /DISCARD/ : {
    *(.eh_frame*)
    *(.note .note.*)
  }
}

