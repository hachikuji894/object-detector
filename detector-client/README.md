Object Detector Client
==== 
- 物体检测客户端，图片将传送给服务端进行处理后显示
## 开发环境
1. 编程语言：C++ 17
2. 编译器：mingw730_64
3. 技术选型：Qt 5.12.12 + OpenCV 4.5.2 + OpenGL 3.3
4. 编程环境：CLion 2020.3.4 + Window 10
## 项目文件
~~~     
├── src            
│    └── assets               // 静态资源文件
│    └── common               // 宏定义
│    └── component            // 注入组件
│    └── controller           // 控制层
│    └── service              // 图像处理
│    └── system               // 系统服务
│    └── utils                // 工具类
│    └── views                // 程序入口
│          └── screen         // 页面
│          └── widget         // 通用 UI 组件
│          └── main.qml       // UI 入口
│    └── main.c               // 程序入口
│    └── resource.qrc         // 资源配置文件
│    └── CMakeLists.txt
├── CMakeLists.txt
├── README.md
~~~
## 本地运行
### 配置 Qt5 环境和 Toolchains
- 安装 Qt 后将  `@/Tools/mingw730_64` 作为 CLion 的 Toolchains，添加环境变量 `@/5.12.12/mingw73_64/bin`， 修改 CMakeLists 中 `CMAKE_PREFIX_PATH ` 为 `@/5.12.12/mingw73_64/lib/cmake`
### 配置 OpenCV 4.5.2 环境
- 下载已编译完成的 MinGW 版 OpenCV 库，并修改 CMakeLists 中 `OpenCV_DIR`，添加环境变量 `@/x64/mingw/bin` 
~~~
git clone -b OpenCV-4.5.2-x64 git@github.com:huihut/OpenCV-MinGW-Build.git
~~~
### 设置网络摄像头
- `https://apkpure.com/cn/` 下载安装 DroidCam 或 IP 摄像头


