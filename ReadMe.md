# PClib

<div style="text-align: center;">

[中文](ReadMe.md) | [English](ReadMe-EN.md)

<!-- 快照 blueviolet，正式版 green -->
![lang](https://img.shields.io/badge/标准-C++23-yellow?logo=cplusplus)
![version](https://img.shields.io/badge/版本-26v4a-blueviolet)
[![github](https://img.shields.io/badge/Github-PClib-blue?&logo=github)](https://github.com/PCwqyy/PCLib)
[![gitee](https://img.shields.io/badge/Gitee-PClib-red?logo=gitee&color=%23C71D23)](https://gitee.com/pcwqyy/PClib)

</div>

这是一个个人 C++ 库，其中某些功能仅在 Windows 上可用

## 功能
- 文本用户界面（TUI）
- 颜色计算
- IO & 文件操作
- Ansi 转义
- Unicode 处理
- _数学支持_
- 容器类
- （Windows） Web 套接字（socket）
- （Windows） 控制台扩展
- 还有更多...

## 安装
克隆仓库并 include 头文件即可

```bash
git clone https://github.com/PCwqyy/PCLib.git
git clone https://gitee.com/pcwqyy/PClib.git
```

示例:

```cpp
#include"/path/to/PCLib/Utility/Ansi.hpp"
```

## 演示

详见[此处](./demo/)

## 联系
作者英语水平有限，如果发现拼错等还请多多包涵

邮箱： w123567910@outlook.com  
博客：[www.cnblogs.com/PCwqyy](https://www.cnblogs.com/PCwqyy)

## 许可证
该项目使用 [MIT License](https://opensource.org/licenses/MIT) 。

## 贡献
欢迎任何形式贡献！您可以通过以下方法参与：
1. 提交 issues 以报告错误或提议新功能
2. 提交 pull request 以修复问题
3. 改进文档

## 版本号
所以，版本号怎么编的？
- 如果是正式版：
  - 采用 `1.X.Y` 格式
  - `X` 大版本号
  - `Y` 小版本号，如为 0 则省去
  - 举例：`1.2` `1.1.7`
- 如果是快照版：
  - 采用 `YYvZZX` 格式 ~~跟 Mojang 学的~~
  - `YY` 年份
  - `ZZ` 年份内大快照编号
  - `X` 大快照内小快照编号
    - 按字母顺序编排
	- 如果本次快照是损坏的、不可运行的，则用 `q`, `qq`, `q3`, `q4` ...标记，这类快照一般没有自述文件
  - 举例：`24v3a` `25v15e` `26v1q` `24v5qq` `24v5q5`

# PClib 1.2.1 快照阶段
- 自动同步数据容器 `SyncedData`
- 标签化错误处理 `pc::Exception`
- 多种文件类型支持 `pc::File`

### 正在开发……
|功能|计划实现版本|子库|
|-|-|-|
|反射序列化器|1.2.2|Experimental|
|反射 Json 解析器|1.2.2|Experimental|
|清理老库中过时的类|1.2.3|Container|
|健壮化老东西|1.2.3|Container|
|重构 TUI 库为 DOM 结构|1.3|TUI|
|重构 Command 库参照 Minecraft 源码|1.3.1|Container|

# 更新日志
[历史](ChangeLogHistory.md)

## 26v1a
- 大幅修改了 [`Exception.hpp`](./src/Exception.hpp)  
现支持流式处理、强制处理、类型继承等功能
## 26v2a
- 拆分 TUI 头文件
- 拆出 [`StrUtils.hpp`](./src/Utility/StrUtils.hpp)  
- 完善 [`Color.hpp`](./src/Container/Color.hpp) 的错误处理机制
- 修改了 [`Exception.hpp`](./src/Exception.hpp)  
现支持类型继承功能支持拥有多个类型同时捕获
### 新 Demo: [Color](./demo/Color/)
- 演示如何使用 `Color` 类
## 26v3a
- 移除了 `Multinclude.hpp`
- 重构了 [`File.hpp`](./src/Utility/File.hpp)
- 完善了常量查询类 [`EnumLookup`](./src/Utility/EnumLookup.hpp)

目前 [`File.hpp`](./src/Utility/File.hpp) 未测试，TUI 因 [`EnumLookup`](./src/Utility/EnumLookup.hpp) 改动二暂时损坏，~~将在下一个快照修复~~
## 26v4a
好吧我要拖一下 TUI 库的修复工作了……
### 重命名 `Unicode.hpp` → `Unicodes.hpp`
- 重构了 [`Unicodes.hpp`](./src/Utility/Unicodes.hpp)：加入了 UTF8、16、32的互相转换与 `std::format` 兼容，并解决了跨平台输出问题
- 准备写 [`Meta.hpp`](./src/Experimental/Meta.hpp)， 将使用C++26反射库
## 26v5a
- 移除了 `VarSet.hpp`, `Sortting.hpp` 以及 `Containers.hpp` 中的不健壮且可被标准库替代的容器
- 移除了 `Window.hpp`，因为我再也不想和WinAPI打交道了

<!--记得改徽章的版本！-->