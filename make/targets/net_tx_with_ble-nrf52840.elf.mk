ifeq (1,$(includecount))

include make/config/target-nrf52840.mk
include make/config/lib-libmira-nrf52840.mk
include make/config/lib-tinyprintf.mk

MODULES += \
	lib/freertos_tools \
	lib/mira_integration \
	lib/newlib_integration \
	lib/uart \
	examples_common \
	net_tx_with_ble

LDFLAGS += \
	-Tsrc/examples_common/example-nrf52840.ld

# NRF ble code needs a specific board defined. Intended to be run on nrf52840-dk
CFLAGS += \
	-DBOARD_PCA10056

# Extra files from nrf5-sdk needed for BLE
SRC += \
	$(NRF5_SDK)/components/ble/ble_advertising/ble_advertising.c \
	$(NRF5_SDK)/components/ble/ble_services/ble_bas/ble_bas.c \
	$(NRF5_SDK)/components/ble/ble_services/ble_dis/ble_dis.c \
	$(NRF5_SDK)/components/ble/ble_services/ble_hrs/ble_hrs.c \
	$(NRF5_SDK)/components/ble/common/ble_advdata.c \
	$(NRF5_SDK)/components/ble/common/ble_conn_params.c \
	$(NRF5_SDK)/components/ble/common/ble_conn_state.c \
	$(NRF5_SDK)/components/ble/common/ble_srv_common.c \
	$(NRF5_SDK)/components/ble/nrf_ble_gatt/nrf_ble_gatt.c \
	$(NRF5_SDK)/components/ble/nrf_ble_qwr/nrf_ble_qwr.c \
	$(NRF5_SDK)/components/ble/peer_manager/gatt_cache_manager.c \
	$(NRF5_SDK)/components/ble/peer_manager/gatts_cache_manager.c \
	$(NRF5_SDK)/components/ble/peer_manager/id_manager.c \
	$(NRF5_SDK)/components/ble/peer_manager/peer_data_storage.c \
	$(NRF5_SDK)/components/ble/peer_manager/peer_database.c \
	$(NRF5_SDK)/components/ble/peer_manager/peer_id.c \
	$(NRF5_SDK)/components/ble/peer_manager/peer_manager.c \
	$(NRF5_SDK)/components/ble/peer_manager/peer_manager_handler.c \
	$(NRF5_SDK)/components/ble/peer_manager/pm_buffer.c \
	$(NRF5_SDK)/components/ble/peer_manager/security_dispatcher.c \
	$(NRF5_SDK)/components/ble/peer_manager/security_manager.c \
	$(NRF5_SDK)/components/boards/boards.c \
	$(NRF5_SDK)/components/libraries/atomic/nrf_atomic.c \
	$(NRF5_SDK)/components/libraries/atomic_fifo/nrf_atfifo.c \
	$(NRF5_SDK)/components/libraries/atomic_flags/nrf_atflags.c \
	$(NRF5_SDK)/components/libraries/bsp/bsp.c \
	$(NRF5_SDK)/components/libraries/button/app_button.c \
	$(NRF5_SDK)/components/libraries/fds/fds.c \
	$(NRF5_SDK)/components/libraries/fstorage/nrf_fstorage.c \
	$(NRF5_SDK)/components/libraries/fstorage/nrf_fstorage_sd.c \
	$(NRF5_SDK)/components/libraries/sensorsim/sensorsim.c \
	$(NRF5_SDK)/components/softdevice/common/nrf_sdh_ble.c \
	$(NRF5_SDK)/modules/nrfx/drivers/src/nrfx_gpiote.c

INC += \
	$(NRF5_SDK)/components/ble/ble_advertising \
	$(NRF5_SDK)/components/ble/ble_services/ble_bas \
	$(NRF5_SDK)/components/ble/ble_services/ble_dis \
	$(NRF5_SDK)/components/ble/ble_services/ble_hrs \
	$(NRF5_SDK)/components/ble/common \
	$(NRF5_SDK)/components/ble/nrf_ble_gatt \
	$(NRF5_SDK)/components/ble/nrf_ble_qwr \
	$(NRF5_SDK)/components/ble/peer_manager \
	$(NRF5_SDK)/components/boards \
	$(NRF5_SDK)/components/libraries/atomic \
	$(NRF5_SDK)/components/libraries/atomic_fifo \
	$(NRF5_SDK)/components/libraries/atomic_flags \
	$(NRF5_SDK)/components/libraries/bsp \
	$(NRF5_SDK)/components/libraries/button \
	$(NRF5_SDK)/components/libraries/fds \
	$(NRF5_SDK)/components/libraries/fstorage \
	$(NRF5_SDK)/components/libraries/mutex \
	$(NRF5_SDK)/components/libraries/sensorsim \
	$(NRF5_SDK)/components/libraries/timer \
	$(NRF5_SDK)/modules/nrfx/hal

APPS += net_tx_with_ble-nrf52840.elf

endif