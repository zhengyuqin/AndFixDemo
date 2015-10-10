测试结果(debug和release都一样)
===

适用情景
---

    * 比之前版本添加class文件,并且调用class.method(),这里的method()是不能用到context的
    * 只能在原来方法上修改
    * 后一个版本可以调用前个版本存在的资源文件
    * out.patch可以是未签名,在release照样适用

不适用情景
---

    * 如果之前不存在该资源文件,无法实现效果但不报错
    * 如果添加了类,而且调用class.method(context),直接崩掉
    * 在原来文件中添加方法,报Fatal signal 11(SIGSEGV),code 2,fault addr .. 错误

其他
---

    * 如果修改AndFix java代码,会报错

注意
---
    * <uses-permission android:name = "android.permission.WRITE_EXTERNAL_STORAGE" />后面必须大写啊.