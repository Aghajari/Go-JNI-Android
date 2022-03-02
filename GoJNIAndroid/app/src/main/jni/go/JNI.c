//
// Created by AmirHossein Aghajari on 3/2/22.
//

#include <jni.h>

extern char *goHelloWorld();
extern int goInteger();

jstring Java_com_aghajari_Native_helloWorld(JNIEnv *env, jclass clazz) {
    return (*env)->NewStringUTF(env, goHelloWorld());
}

jint Java_com_aghajari_Native_integerTest(JNIEnv *env, jclass clazz) {
    return goInteger();
}