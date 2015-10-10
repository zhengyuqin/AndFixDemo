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

使用流程
===

生成apk
---

    * 编译安装1.apk
    * ./gradlew clean assembleRelease 生成2.apk,注意文件夹META-INF,smali的变化

使用apkpatch工具(据说这个到时会开源)
---

    * 把1.apk,2.apk放到含有apkpatch.sh,apkpatch-1.0.3.jar的文件夹中
    * ./apkpatch.sh 查看用法
    * 把生成.patch文件命名为out.patch,放到sd0的根目录中,清掉1.apk的缓存数据,重启1.apk就可以看到效果了
