#编译动态库
#LOCAL_PATH用于在开发树中找到源代码,宏函数'my-dir'就是指Android.mk file文件所在的目录
LOCAL_PATH:= $(call my-dir)
#告诉编译系统,清除掉所有许多LOCAL_XXX变量(包括下面这些字段)
include $(CLEAR_VARS)

#变量必须包含将要编译打包模块中的C/C++,不用列出头文件和包含文件
LOCAL_SRC_FILES:= andfix.cpp\
					art/art_method_replace.cpp \
					art/art_method_replace_5_0.cpp \
					art/art_method_replace_5_1.cpp \
					art/art_method_replace_6_0.cpp \
					dalvik/dalvik_method_replace.cpp \

#可选的编辑器选项,在编译C代码文件时候使用
LOCAL_CFLAGS	:= -std=gnu++11 -fpermissive -DDEBUG -O0

LOCAL_C_INCLUDES := 

LOCAL_SHARED_LIBRARIES := 

#编译你的模块要使用的附加的链接器选项,这对于使用"-l"前缀传递指定库的名字是可用的,告诉链接器生成的模块在加载时候要链接到/system/lib/log
LOCAL_LDLIBS    := -llog

LOCAL_STATIC_LIBRARIES :=

#名称是唯一的,且不包含任何空格,注意编译系统会自动产生合适的前缀和后缀,将会生成libandfix.so文件
LOCAL_MODULE:= andfix


include $(BUILD_SHARED_LIBRARY)
