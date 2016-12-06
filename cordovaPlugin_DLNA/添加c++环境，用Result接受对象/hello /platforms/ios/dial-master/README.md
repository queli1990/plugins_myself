DIAL Client
===============

BUILD chrome nacl
===============
1. Init nacl_sdk
cd tools/nacl_sdk
./naclsdk update pepper_49

2. Compile
make


BUILD android .so
===============
1. 先初始化安卓NDK编译工具链,首先你需要有NDK套件,可到android管方网站下载,准备好后执行
./tools/install-android-toolchain arm NDK_DIR    or
./tools/install-android-toolchain x86 NDK_DIR

2.编译
make android | android-arm | android-x86
编译输出文件在out/android/*/libdialclient.so

3.Java调用Native接口
可参见android.cc文件中供的C函数,各函数返回均为string值部分函数返回JSON格式字符串。
具体使用方法可参照android JNI

4.现在out目录下有编译好的arm/x86版本的so文件



BUILD ios
===============
工程目录下有diaclient.xcodeeproj文件,具体项目可包含该文件并使用其中的静态库项目。
使用时需要把工具目录添加为C++头文件搜索路径
