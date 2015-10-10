.class public Lcom/zyq/andfixdemo/MainActivity_CF;
.super Landroid/app/Activity;
.source "MainActivity.java"


# static fields
.field private static final TFG:Ljava/lang/String; = "euler"


# direct methods
.method public constructor <init>()V
    .locals 0

    .prologue
    .line 12
    invoke-direct {p0}, Landroid/app/Activity;-><init>()V

    return-void
.end method


# virtual methods
.method protected onCreate(Landroid/os/Bundle;)V
    .locals 5
    .param p1, "savedInstanceState"    # Landroid/os/Bundle;
    .annotation runtime Lcom/alipay/euler/andfix/annotation/MethodReplace;
        clazz = "com.zyq.andfixdemo.MainActivity"
        method = "onCreate"
    .end annotation

    .prologue
    .line 18
    invoke-super {p0, p1}, Landroid/app/Activity;->onCreate(Landroid/os/Bundle;)V

    .line 19
    const-string v0, "euler"

    const-string v1, "good"

    invoke-static {v1}, Lcom/zyq/test/Fix;->a(Ljava/lang/String;)Ljava/lang/String;

    move-result-object v1

    invoke-static {v0, v1}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I

    .line 20
    const-string v0, "euler"

    new-instance v1, Ljava/lang/StringBuilder;

    invoke-direct {v1}, Ljava/lang/StringBuilder;-><init>()V

    const-string v2, ""

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(Ljava/lang/String;)Ljava/lang/StringBuilder;

    move-result-object v1

    new-instance v2, Lcom/zyq/test/Fix;

    invoke-direct {v2}, Lcom/zyq/test/Fix;-><init>()V

    const-string v3, "s1"

    const-string v4, "s2"

    invoke-virtual {v2, v3, v4}, Lcom/zyq/test/Fix;->b(Ljava/lang/String;Ljava/lang/String;)I

    move-result v2

    invoke-virtual {v1, v2}, Ljava/lang/StringBuilder;->append(I)Ljava/lang/StringBuilder;

    move-result-object v1

    invoke-virtual {v1}, Ljava/lang/StringBuilder;->toString()Ljava/lang/String;

    move-result-object v1

    invoke-static {v0, v1}, Landroid/util/Log;->e(Ljava/lang/String;Ljava/lang/String;)I

    .line 21
    invoke-virtual {p0}, Lcom/zyq/andfixdemo/MainActivity_CF;->getApplicationContext()Landroid/content/Context;

    move-result-object v0

    const-string v1, "helloworld"

    const/4 v2, 0x1

    invoke-static {v0, v1, v2}, Landroid/widget/Toast;->makeText(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;

    move-result-object v0

    invoke-virtual {v0}, Landroid/widget/Toast;->show()V

    .line 22
    invoke-static {}, Lcom/alipay/euler/andfix/Util_LOGFile;->getInstance()Lcom/alipay/euler/andfix/Util_LOGFile;

    move-result-object v0

    const-string v1, "eulerhelloWorld"

    invoke-virtual {v0, v1}, Lcom/alipay/euler/andfix/Util_LOGFile;->write(Ljava/lang/String;)V

    .line 23
    return-void
.end method

.method protected onDestroy()V
    .locals 1

    .prologue
    .line 27
    invoke-super {p0}, Landroid/app/Activity;->onDestroy()V

    .line 28
    invoke-static {}, Landroid/os/Process;->myPid()I

    move-result v0

    invoke-static {v0}, Landroid/os/Process;->killProcess(I)V

    .line 29
    return-void
.end method
