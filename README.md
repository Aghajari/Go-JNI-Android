# Go-JNI-Android
 A template project to use go as a native library in android projects

1. First of all, you should add JNI support to your project. [Official Tutorial](https://developer.android.com/studio/projects/add-native-code) 
2. Create `jni`/`cpp` folder (New -> Folder -> JNI/CPP Folder)
3. Create a `.c`/`.cpp` file in jni/cpp for creating a c library (for example: `c_android.c`)
4. Create `CMakeFiles.txt` in jni/cpp
```cmake
cmake_minimum_required(VERSION 3.4.1)

SET(C_LIB native-lib)

add_library(${C_LIB}
        SHARED
        c_android.c)

find_library(log-lib log)
target_link_libraries(${C_LIB} ${log-lib})
```
5. Add `externalNativeBuild` to build.gradle (Module: app)
```gradle
buildTypes {
    ...
    
    externalNativeBuild {
        cmake {
            path "src/main/jni/CMakeLists.txt" // jni/cpp
        }
    }
}
```
6. You should be able to build and compile your project if you have done everything successfully!

7. Install [Go](https://go.dev/doc/install) (If you haven't done it already)
> Verify that you've installed Go by opening a command prompt and typing the following command:
```
$ go version
```
8. Create a folder named `go` for go sources
9. Ceate `makefile` in go with following codes
```make
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
```
10. Edit `CMakeFiles.txt` to include go as a target
```cmake
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
```

11. Create a `.go` file in `go` folder
```go
package main

import "C"

// export functionName => makes function accessible in c

//export goHelloWorld
func goHelloWorld() *C.char {
    return C.CString("Hello :)")
}

//export goInteger
func goInteger() int {
    return 24
}

func main() {}
```

12. Create a `.c`/`.cpp` file for connecting go to jni (I suggest you name it JNI.c)
```c
#include <jni.h>

extern char *goHelloWorld();
extern int goInteger();

jstring Java_packagename_ClassName_helloWorld(JNIEnv *env, jclass clazz) {
    return (*env)->NewStringUTF(env, goHelloWorld());
}

jint Java_packagename_ClassName_integerTest(JNIEnv *env, jclass clazz) {
    return goInteger();
}
```
13. Create the java class
```java
package packagename;

public class ClassName {
    static {
        System.loadLibrary("native-lib-go"); // CMakeLists.txt => ${GO_Lib}
    }
    
    public static native String helloWorld();
    public static native int integerTest();
}
```
14. And everything is done!


You can also directly link java & go function like this :
```go
//export Java_packagename_ClassName_goTest
func Java_packagename_ClassName_goTest(env uintptr, clazz uintptr) int {
    return 100
}
```
```java
public static native int goTest();
```
