package main

import "C"

//export goHelloWorld
func goHelloWorld() *C.char {
    return C.CString("Hello :)")
}

//export goInteger
func goInteger() int {
    return 24
}

func main() {}