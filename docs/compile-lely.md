CFLAGS="-g -O2 -fno-builtin -mfloat-abi=hard -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16" \
                                                    CXXFLAGS="-g -O2 -fno-builtin -mfloat-abi=hard -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16" \
                                                    LDFLAGS="--specs=nosys.specs" \
                                                    ../configure --host=arm-none-eabi \
                                                      --disable-shared \
                                                      --disable-tools \
                                                      --disable-tests \
                                                      --disable-python \
                                                      --disable-threads \
                                                      --disable-daemon

make install DESTDIR=$(pwd)/../install

libtool: warning: remember to run 'libtool --finish /usr/local/lib'

funktioninert nicht:


include(ExternalProject)

set(LELY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lely-core)
set(LELY_INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/out/external)
set(CFLAGS "-mfloat-abi=hard -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16  -mfloat-abi=hard")
set(LDFLAGS "-mfloat-abi=hard -mcpu=cortex-m4 -mthumb  -mfpu=fpv4-sp-d16  -mfloat-abi=hard --specs=nosys.specs")


ExternalProject_Add(lely-core
  SOURCE_DIR ${LELY_SRC}
  BINARY_DIR ${LELY_SRC}
  SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/lely-core
  DOWNLOAD_COMMAND ""
  CONFIGURE_COMMAND
  env
    CC=arm-none-eabi-gcc
    AR=arm-none-eabi-ar
    RANLIB=arm-none-eabi-ranlib

  ./configure
  --host=arm-none-eabi 
  --prefix=${CMAKE_CURRENT_SOURCE_DIR}/out/external 
  --disable-shared 
  --disable-tools 
  --disable-tests 
  --disable-python 
  --disable-threads 
  --disable-daemon
  
  BUILD_COMMAND make
  INSTALL_COMMAND make install
)
ExternalProject_Get_Property(lely-core SOURCE_DIR)
#message("+++++" ${SOURCE_DIR})
