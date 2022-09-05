exec make SDK_ROOT=/home/sebastian/src/mira/vendor/nrf5-sdk/ \
  GNU_INSTALL_ROOT=$(dirname $(which arm-none-eabi-gcc))/ \
  MIRA_LIB=/home/sebastian/src/mira/build/libmira/ $*

  MIRA_LIB=/home/sebastian/src/miramesh-freertos-examples/libmira/ $*



