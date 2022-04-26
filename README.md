# 文件结构
## main.cpp
### camera.cpp
调用`image_process_function.cpp`
### planner.cpp
### control.cpp
调用`arm_control_function.cpp`

## 分工
我已经写好了文件的整体架构和大部分的接口以及注释，之后再填一下camera.cpp和planner.cpp的内容。但由于还没有拿到相机，camera没法完全完成。
- xd：参考socket通讯的语法，并查阅UR机械臂socket通讯的格式，写一下arm_control_function.cpp.目前至少要包括两个函数：设置机械臂末端的位姿，读取机械臂末端的位姿。形式上可以参考`相关文件/大象机械臂控制函数库.py`,形式上时类似的。
- zx：写一下control.cpp里的两个函数。Glue函数，接口已经写的非常清楚了，调用xd写的arm_control_function.cpp里设置机械臂末端位姿的函数就好了。两个人商量一下接口。因为是平面涂胶，z是一个值，姿态也是竖直，固定不变。InitArm函数，初始化机械臂设置，需要查阅一些ur资料。
