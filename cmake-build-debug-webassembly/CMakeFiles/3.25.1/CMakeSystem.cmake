set(CMAKE_HOST_SYSTEM "Linux-6.1.0-32-amd64")
set(CMAKE_HOST_SYSTEM_NAME "Linux")
set(CMAKE_HOST_SYSTEM_VERSION "6.1.0-32-amd64")
set(CMAKE_HOST_SYSTEM_PROCESSOR "x86_64")

include("/usr/local/Qt-6.8.2-wasm-multithread/lib/cmake/Qt6/qt.toolchain.cmake")

set(CMAKE_SYSTEM "Emscripten-1")
set(CMAKE_SYSTEM_NAME "Emscripten")
set(CMAKE_SYSTEM_VERSION "1")
set(CMAKE_SYSTEM_PROCESSOR "x86")

set(CMAKE_CROSSCOMPILING "TRUE")

set(CMAKE_SYSTEM_LOADED 1)
