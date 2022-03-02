NDK_GO_ARCH_x86 := 386
NDK_GO_ARCH_x86_64 := amd64
NDK_GO_ARCH_arm := arm
NDK_GO_ARCH_arm64 := arm64
NDK_GO_ARCH_mips := mipsx
NDK_GO_ARCH_mips64 := mips64x

CLANG_FLAGS := --target=$(ANDROID_LLVM_TRIPLE) --gcc-toolchain=$(ANDROID_TOOLCHAIN_ROOT) --sysroot=$(ANDROID_SYSROOT)
export CGO_CFLAGS := $(CLANG_FLAGS) $(CFLAGS)
export CGO_LDFLAGS := $(CLANG_FLAGS) $(LDFLAGS) -Wl,-soname=${SONAME}
export CC := $(ANDROID_C_COMPILER)
export GOARCH := $(NDK_GO_ARCH_$(ANDROID_ARCH_NAME))
export GOOS := android
export CGO_ENABLED := 1

$(BUILDDIR)/${SONAME}:
	go build -o "$(BUILDDIR)/${SONAME}" -buildmode c-shared

.DELETE_ON_ERROR: