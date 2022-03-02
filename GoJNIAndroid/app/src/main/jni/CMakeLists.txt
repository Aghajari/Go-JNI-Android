cmake_minimum_required(VERSION 3.4.1)

SET(C_LIB native-lib)
SET(GO_LIB native-lib-go)

add_library(${C_LIB}
        SHARED
        c_android.c)

add_custom_target(${GO_LIB}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/go"
        COMMENT "Building ${GO_LIB}"
        VERBATIM COMMAND make
        ANDROID_ARCH_NAME=${ANDROID_ARCH_NAME}
        ANDROID_C_COMPILER=${ANDROID_C_COMPILER}
        ANDROID_TOOLCHAIN_ROOT=${ANDROID_TOOLCHAIN_ROOT}
        ANDROID_LLVM_TRIPLE=${ANDROID_LLVM_TRIPLE}
        ANDROID_SYSROOT=${ANDROID_SYSROOT}
        ANDROID_PACKAGE_NAME=${ANDROID_PACKAGE_NAME}
        GRADLE_USER_HOME=${GRADLE_USER_HOME}
        CFLAGS=${CMAKE_C_FLAGS}\ -Wno-unused-command-line-argument
        LDFLAGS=${CMAKE_SHARED_LINKER_FLAGS}\ -fuse-ld=gold
        BUILDDIR=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        SONAME=lib${GO_LIB}.so)

add_dependencies(${C_LIB} ${GO_LIB})

find_library(log-lib log)
target_link_libraries(${C_LIB} ${log-lib})