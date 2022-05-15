Object Detector
==== 
- 基于 Qt 和 Pytorch 的物体检测系统，检测网络使用针对小样本问题基于 maskrcnn_benchmark 改进的 MPSR。客户端用 C++ 编写，后端服务用 Python 的 Flask 快速开发。

- 前端和后端分离目的在于，网络模型可以方便的替换、避开由于 Pytorch 模型生成 TorchScript 有诸多限制导致在 C++ 部署困难的问题、使 C++ 编写的 Qt 桌面端应用能通过 resful 接口方便的调用 Pytorch 编写的网络模型进行检测。

- 基于 Pytorch 检测网络模型参考

~~~
https://github.com/jiaxi-wu/MPSR
~~~
- 基于 QML 前端界面参考
~~~
https://github.com/MattLigocki/DNNAssist
https://github.com/NovemberJ/qt-object-measure
~~~
## 项目文件
~~~     
├── object-detector           
│    └── detector-client
│    └── detector-server
~~~
