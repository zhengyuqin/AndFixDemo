/*
 *
 * Copyright (c) 2015, alipay.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * 	dalvik_method_replace.cpp
 *
 * @author : sanping.li@alipay.com
 *
 */

/**
 * 修改特定方法所对应的Method结构体中的变量来实现对Dalvik层方法的hook的
 */
#include <time.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include <stdbool.h>
#include <fcntl.h>
#include <dlfcn.h>

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "common.h"
#include "dalvik.h"

static JNIEnv *jni_env;
static ClassObject *classJavaLangObjectArray;
static jclass NPEClazz;
static jclass CastEClazz;
static jmethodID jInvokeMethod;
static jmethodID jClassMethod;

/**
 * __attribute__的作用是告诉编译器取消结构在编译过程中的优化对齐，按照实际占用字节数进行对齐，是GCC特有的语法
 *　对于_Z32dvmCreateStringFromCstrAndLengthPKcj这个函数名来说，
 * 最前面的_Z说明这是一个全局函数（即不属于任何类，并且在顶层命名空间内）；
 * 32说明真正的函数名有32个字符；接下来就是真实的函数名，即dvmCreateStringFromCstrAndLength，
 * 刚好是32个字符；再下来就是参数的类型信息，Pkc代表函数的第一个参数的类型是const char*，
 * j代表第二个参数的类型是u4。如果大家想进一步了解关于符号重编的具体细节
 * http://blog.csdn.net/roland_sun/article/details/43233565
 *
 */
extern jboolean __attribute__ ((visibility ("hidden"))) dalvik_setup(
        JNIEnv *env, int apilevel) {
    jni_env = env;
    //dlopen:打开一个动态链接库，把libdvm.so加载到目标进程，算是是注入
    void *dvm_hand = dlopen("libdvm.so", RTLD_NOW);
    if (dvm_hand) {
        //计算这个方法需要的参数大小
        dvmComputeMethodArgsSize_fnPtr = dvm_dlsym(dvm_hand,
                                                   apilevel > 10 ?
                                                   "_Z24dvmComputeMethodArgsSizePK6Method" :
                                                   "dvmComputeMethodArgsSize");
        if (!dvmComputeMethodArgsSize_fnPtr) {
            throwNPE(env, "dvmComputeMethodArgsSize_fnPtr");
            return JNI_FALSE;
        }
        //找到该方法的指针
        dvmCallMethod_fnPtr = dvm_dlsym(dvm_hand,
                                        apilevel > 10 ?
                                        "_Z13dvmCallMethodP6ThreadPK6MethodP6ObjectP6JValuez" :
                                        "dvmCallMethod");
        if (!dvmCallMethod_fnPtr) {
            throwNPE(env, "dvmCallMethod_fnPtr");
            return JNI_FALSE;
        }
        dexProtoGetParameterCount_fnPtr = dvm_dlsym(dvm_hand,
                                                    apilevel > 10 ?
                                                    "_Z25dexProtoGetParameterCountPK8DexProto" :
                                                    "dexProtoGetParameterCount");
        if (!dexProtoGetParameterCount_fnPtr) {
            throwNPE(env, "dexProtoGetParameterCount_fnPtr");
            return JNI_FALSE;
        }

        dvmAllocArrayByClass_fnPtr = dvm_dlsym(dvm_hand,
                                               "dvmAllocArrayByClass");
        if (!dvmAllocArrayByClass_fnPtr) {
            throwNPE(env, "dvmAllocArrayByClass_fnPtr");
            return JNI_FALSE;
        }
        dvmBoxPrimitive_fnPtr = dvm_dlsym(dvm_hand,
                                          apilevel > 10 ?
                                          "_Z15dvmBoxPrimitive6JValueP11ClassObject" :
                                          "dvmWrapPrimitive");
        if (!dvmBoxPrimitive_fnPtr) {
            throwNPE(env, "dvmBoxPrimitive_fnPtr");
            return JNI_FALSE;
        }
        dvmFindPrimitiveClass_fnPtr = dvm_dlsym(dvm_hand,
                                                apilevel > 10 ?
                                                "_Z21dvmFindPrimitiveClassc" : "dvmFindPrimitiveClass");
        if (!dvmFindPrimitiveClass_fnPtr) {
            throwNPE(env, "dvmFindPrimitiveClass_fnPtr");
            return JNI_FALSE;
        }
        dvmReleaseTrackedAlloc_fnPtr = dvm_dlsym(dvm_hand,
                                                 "dvmReleaseTrackedAlloc");
        if (!dvmReleaseTrackedAlloc_fnPtr) {
            throwNPE(env, "dvmReleaseTrackedAlloc_fnPtr");
            return JNI_FALSE;
        }
        dvmCheckException_fnPtr = dvm_dlsym(dvm_hand,
                                            apilevel > 10 ?
                                            "_Z17dvmCheckExceptionP6Thread" : "dvmCheckException");
        if (!dvmCheckException_fnPtr) {
            throwNPE(env, "dvmCheckException_fnPtr");
            return JNI_FALSE;
        }

        dvmGetException_fnPtr = dvm_dlsym(dvm_hand,
                                          apilevel > 10 ?
                                          "_Z15dvmGetExceptionP6Thread" : "dvmGetException");
        if (!dvmGetException_fnPtr) {
            throwNPE(env, "dvmGetException_fnPtr");
            return JNI_FALSE;
        }
        dvmFindArrayClass_fnPtr = dvm_dlsym(dvm_hand,
                                            apilevel > 10 ?
                                            "_Z17dvmFindArrayClassPKcP6Object" :
                                            "dvmFindArrayClass");
        if (!dvmFindArrayClass_fnPtr) {
            throwNPE(env, "dvmFindArrayClass_fnPtr");
            return JNI_FALSE;
        }
        dvmCreateReflectMethodObject_fnPtr = dvm_dlsym(dvm_hand,
                                                       apilevel > 10 ?
                                                       "_Z28dvmCreateReflectMethodObjectPK6Method" :
                                                       "dvmCreateReflectMethodObject");
        if (!dvmCreateReflectMethodObject_fnPtr) {
            throwNPE(env, "dvmCreateReflectMethodObject_fnPtr");
            return JNI_FALSE;
        }

        dvmGetBoxedReturnType_fnPtr = dvm_dlsym(dvm_hand,
                                                apilevel > 10 ?
                                                "_Z21dvmGetBoxedReturnTypePK6Method" :
                                                "dvmGetBoxedReturnType");
        if (!dvmGetBoxedReturnType_fnPtr) {
            throwNPE(env, "dvmGetBoxedReturnType_fnPtr");
            return JNI_FALSE;
        }
        dvmUnboxPrimitive_fnPtr = dvm_dlsym(dvm_hand,
                                            apilevel > 10 ?
                                            "_Z17dvmUnboxPrimitiveP6ObjectP11ClassObjectP6JValue" :
                                            "dvmUnwrapPrimitive");
        if (!dvmUnboxPrimitive_fnPtr) {
            throwNPE(env, "dvmUnboxPrimitive_fnPtr");
            return JNI_FALSE;
        }
        dvmDecodeIndirectRef_fnPtr = dvm_dlsym(dvm_hand,
                                               apilevel > 10 ?
                                               "_Z20dvmDecodeIndirectRefP6ThreadP8_jobject" :
                                               "dvmDecodeIndirectRef");
        if (!dvmDecodeIndirectRef_fnPtr) {
            throwNPE(env, "dvmDecodeIndirectRef_fnPtr");
            return JNI_FALSE;
        }
        dvmThreadSelf_fnPtr = dvm_dlsym(dvm_hand,
                                        apilevel > 10 ? "_Z13dvmThreadSelfv" : "dvmThreadSelf");
        if (!dvmThreadSelf_fnPtr) {
            throwNPE(env, "dvmThreadSelf_fnPtr");
            return JNI_FALSE;
        }

        classJavaLangObjectArray = dvmFindArrayClass_fnPtr(
                "[Ljava/lang/Object;", NULL);
        //调用java层的method类
        jclass clazz = env->FindClass("java/lang/reflect/Method");
        //利用反射获取Method的invoke()方法
        jInvokeMethod = env->GetMethodID(clazz, "invoke",
                                         "(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;");
        //利用反射获取Method的getDeclaringClass()方法
        jClassMethod = env->GetMethodID(clazz, "getDeclaringClass",
                                        "()Ljava/lang/Class;");
        NPEClazz = env->FindClass("java/lang/NullPointerException");
        CastEClazz = env->FindClass("java/lang/ClassCastException");
        return JNI_TRUE;
    } else {
        return JNI_FALSE;
    }
}

static void throwNPE(JNIEnv *env, const char *msg) {
    LOGE("setup error: %s", msg);
//	env->ThrowNew(NPEClazz, msg);
}

static bool dvmIsStaticMethod(const Method *method) {
    return (method->accessFlags & ACC_STATIC) != 0;
}

extern void __attribute__ ((visibility ("hidden"))) dalvik_replaceMethod(
        JNIEnv *env, jobject src, jobject dest) {

    //CallObjectMethod:调用一个由methodID定义的实例的Java方法,就是类名
    jobject clazz = env->CallObjectMethod(dest, jClassMethod);
    //获取注册JNI方法的类对象
    ClassObject *clz = (ClassObject *) dvmDecodeIndirectRef_fnPtr(
            dvmThreadSelf_fnPtr(), clazz);
    clz->status = CLASS_INITIALIZED;

    Method *meth = (Method *) env->FromReflectedMethod(src);
    Method *target = (Method *) env->FromReflectedMethod(dest);
    LOGD("dalvikMethod: %s", meth->name);

    meth->jniArgInfo = 0x80000000;
    meth->accessFlags |= ACC_NATIVE;//把java方法变成本地方法

    int argsSize = dvmComputeMethodArgsSize_fnPtr(meth);
    if (!dvmIsStaticMethod(meth))
        argsSize++;
    meth->registersSize = meth->insSize = argsSize;//参数的数量没有改变
    meth->insns = (void *) target;//改变了JNI实际函数机器码的首地址

    meth->nativeFunc = dalvik_dispatcher;//不是jvm的native方法．
}

extern void dalvik_setFieldFlag(JNIEnv *env, jobject field) {
    //这里改写accessFlags,把需要替换的类的所有方法（Field）改成public
    Field *dalvikField = (Field *) env->FromReflectedField(field);
    dalvikField->accessFlags = dalvikField->accessFlags & (~ACC_PRIVATE)
                               | ACC_PUBLIC;
    LOGD("dalvik_setFieldFlag: %d ", dalvikField->accessFlags);
}

static bool dvmIsPrimitiveClass(const ClassObject *clazz) {
    return clazz->primitiveType != PRIM_NOT;
}

static void dalvik_dispatcher(const u4 *args, jvalue *pResult,
                              const Method *method, void *self) {
    ClassObject *returnType;
    jvalue result;
    ArrayObject *argArray;

    LOGD("dalvik_dispatcher source method: %s %s", method->name,
         method->shorty);
    Method *meth = (Method *) method->insns;
    meth->accessFlags = meth->accessFlags | ACC_PUBLIC;
    LOGD("dalvik_dispatcher target method: %s %s", method->name,
         method->shorty);

    returnType = dvmGetBoxedReturnType_fnPtr(method);
    if (returnType == NULL) {
        assert(dvmCheckException_fnPtr(self));
        goto bail;
    }
    LOGD("dalvik_dispatcher start call->");

    if (!dvmIsStaticMethod(meth)) {
        Object *thisObj = (Object *) args[0];
        ClassObject *tmp = thisObj->clazz;
        thisObj->clazz = meth->clazz;
        argArray = boxMethodArgs(meth, args + 1);
        if (dvmCheckException_fnPtr(self))
            goto bail;

        dvmCallMethod_fnPtr(self, (Method *) jInvokeMethod,
                            dvmCreateReflectMethodObject_fnPtr(meth), &result, thisObj,
                            argArray);

        thisObj->clazz = tmp;
    } else {
        argArray = boxMethodArgs(meth, args);
        if (dvmCheckException_fnPtr(self))
            goto bail;

        dvmCallMethod_fnPtr(self, (Method *) jInvokeMethod,
                            dvmCreateReflectMethodObject_fnPtr(meth), &result, NULL,
                            argArray);
    }
    if (dvmCheckException_fnPtr(self)) {
        Object *excep = dvmGetException_fnPtr(self);
        jni_env->Throw((jthrowable) excep);
        goto bail;
    }

    if (returnType->primitiveType == PRIM_VOID) {
        LOGD("+++ ignoring return to void");
    } else if (result.l == NULL) {
        if (dvmIsPrimitiveClass(returnType)) {
            jni_env->ThrowNew(NPEClazz, "null result when primitive expected");
            goto bail;
        }
        pResult->l = NULL;
    } else {
        if (!dvmUnboxPrimitive_fnPtr(result.l, returnType, pResult)) {
            char msg[1024] = {0};
            snprintf(msg, sizeof(msg) - 1, "%s!=%s\0",
                     ((Object *) result.l)->clazz->descriptor,
                     returnType->descriptor);
            jni_env->ThrowNew(CastEClazz, msg);
            goto bail;
        }
    }

    bail:
    dvmReleaseTrackedAlloc_fnPtr((Object *) argArray, self);
}

//dlsym 根据动态链接库 操作句柄(handle)与符号,找到方法的地址和变量
static void *dvm_dlsym(void *hand, const char *name) {
    void *ret = dlsym(hand, name);
    char msg[1024] = {0};
    snprintf(msg, sizeof(msg) - 1, "0x%x", ret);
    LOGD("%s = %s\n", name, msg);
    return ret;
}

static s8 dvmGetArgLong(const u4 *args, int elem) {
    s8 val;
    memcpy(&val, &args[elem], sizeof(val));
    return val;
}

/*
 * Return a new Object[] array with the contents of "args".  We determine
 * the number and types of values in "args" based on the method signature.
 * Primitive types are boxed.
 *
 * Returns NULL if the method takes no arguments.
 *
 * The caller must call dvmReleaseTrackedAlloc() on the return value.
 *
 * On failure, returns with an appropriate exception raised.
 */
static ArrayObject *boxMethodArgs(const Method *method, const u4 *args) {
    const char *desc = &method->shorty[1]; // [0] is the return type.

    /* count args */
    size_t argCount = dexProtoGetParameterCount_fnPtr(&method->prototype);

    /* allocate storage */
    ArrayObject *argArray = dvmAllocArrayByClass_fnPtr(classJavaLangObjectArray,
                                                       argCount, ALLOC_DEFAULT);
    if (argArray == NULL)
        return NULL;
    Object **argObjects = (Object * *)(void * )
    argArray->contents;

    /*
     * Fill in the array.
     */

    size_t srcIndex = 0;
    size_t dstIndex = 0;
    while (*desc != '\0') {
        char descChar = *(desc++);
        jvalue value;

        switch (descChar) {
            case 'Z':
            case 'C':
            case 'F':
            case 'B':
            case 'S':
            case 'I':
                value.i = args[srcIndex++];
                argObjects[dstIndex] = (Object *) dvmBoxPrimitive_fnPtr(value,
                                                                        dvmFindPrimitiveClass_fnPtr(descChar));
                /* argObjects is tracked, don't need to hold this too */
                dvmReleaseTrackedAlloc_fnPtr(argObjects[dstIndex], NULL);
                dstIndex++;
                break;
            case 'D':
            case 'J':
                value.j = dvmGetArgLong(args, srcIndex);
                srcIndex += 2;
                argObjects[dstIndex] = (Object *) dvmBoxPrimitive_fnPtr(value,
                                                                        dvmFindPrimitiveClass_fnPtr(descChar));
                dvmReleaseTrackedAlloc_fnPtr(argObjects[dstIndex], NULL);
                dstIndex++;
                break;
            case '[':
            case 'L':
                argObjects[dstIndex++] = (Object *) args[srcIndex++];
                LOGD("boxMethodArgs object: index = %d", dstIndex - 1);
                break;
        }
    }

    return argArray;
}

