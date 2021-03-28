/* Linker script from nRF52 SDK, modified by LumenRadio AB for MiraMesh usage */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  /* Reserve flash space for Softdevice and MBR: */
  MBR_SD (rx)          : ORIGIN = 0x00000, LENGTH = 0x27000
  FLASH (rx)           : ORIGIN = 0x27000, LENGTH = 0x57000
  /* Store the Certification in the same space as with MiraOS: */
  CERTIFICATE (r)      : ORIGIN = 0x000FF000, LENGTH = 0x1000
  /* Reserve RAM space for Softdevice and MBR: */
  SOFTDEVRAM (rwx)     : ORIGIN = 0x20000000, LENGTH = 0x2300
  RAM (rwx)            : ORIGIN = 0x20002300, LENGTH = 0x3dd00
}

SECTIONS
{
}

SECTIONS
{
  . = ALIGN(4);
  .mem_section_dummy_ram :
  {
  }
  .log_dynamic_data :
  {
    PROVIDE(__start_log_dynamic_data = .);
    KEEP(*(SORT(.log_dynamic_data*)))
    PROVIDE(__stop_log_dynamic_data = .);
  } > RAM
  .log_filter_data :
  {
    PROVIDE(__start_log_filter_data = .);
    KEEP(*(SORT(.log_filter_data*)))
    PROVIDE(__stop_log_filter_data = .);
  } > RAM

} INSERT AFTER .data;

SECTIONS
{
  .mem_section_dummy_rom :
  {
  }
  .log_const_data :
  {
    PROVIDE(__start_log_const_data = .);
    KEEP(*(SORT(.log_const_data*)))
    PROVIDE(__stop_log_const_data = .);
  } > FLASH
  .log_backends :
  {
    PROVIDE(__start_log_backends = .);
    KEEP(*(SORT(.log_backends*)))
    PROVIDE(__stop_log_backends = .);
  } > FLASH
    .nrf_balloc :
  {
    PROVIDE(__start_nrf_balloc = .);
    KEEP(*(.nrf_balloc))
    PROVIDE(__stop_nrf_balloc = .);
  } > FLASH

  /* Add identifiers for Softdevice event observers: */
    .sdh_soc_observers :
  {
    PROVIDE(__start_sdh_soc_observers = .);
    KEEP(*(SORT(.sdh_soc_observers*)))
    PROVIDE(__stop_sdh_soc_observers = .);
  } > FLASH
    .sdh_stack_observers :
  {
    PROVIDE(__start_sdh_stack_observers = .);
    KEEP(*(SORT(.sdh_stack_observers*)))
    PROVIDE(__stop_sdh_stack_observers = .);
  } > FLASH
  .sdh_req_observers :
  {
    PROVIDE(__start_sdh_req_observers = .);
    KEEP(*(SORT(.sdh_req_observers*)))
    PROVIDE(__stop_sdh_req_observers = .);
  } > FLASH

  .sdh_state_observers :
  {
    PROVIDE(__start_sdh_state_observers = .);
    KEEP(*(SORT(.sdh_state_observers*)))
    PROVIDE(__stop_sdh_state_observers = .);
  } > FLASH
} INSERT AFTER .text

/* Create identifiers referencing the MiraMesh's certificate storage: */
__CertificateStart = ORIGIN(CERTIFICATE);
__CertificateEnd = ORIGIN(CERTIFICATE) + LENGTH(CERTIFICATE);

INCLUDE "nrf_common.ld"
