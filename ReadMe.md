# PClib

<div style="text-align: center;">

[中文](ReadMe.md) | [English](ReadMe-EN.md)

<!-- 快照 blueviolet，正式版 green -->
![lang](https://img.shields.io/badge/标准-C++23-yellow?logo=cplusplus)
![version](https://img.shields.io/badge/版本-26v1a-blueviolet)
[![github](https://img.shields.io/badge/Github-PClib-blue?&logo=github)](https://github.com/PCwqyy/PCLib)
[![gitee](https://img.shields.io/badge/Gitee-PClib-red?logo=gitee&color=%23C71D23)](https://gitee.com/pcwqyy/PClib)

</div>

这是一个个人 C++ 库，其中某些功能仅在 Windows 上可用

## 功能
- 文本用户界面（TUI）
- 容器类
- IO & 文件操作
- Ansi 转义
- 一些算法
- 数学支持
- 颜色计算
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
#include"/path/to/PCLib/TUI/TUI.hpp"
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

# PClib 1.2.1（未发布）
- TUI 库的 DOM 结构（这并不是一个小改动）
- 自动同步数据容器 `SyncedData`

# 更新日志
[历史](ChangeLogHistory.md)

## 26v1a
- 大幅修改了 [`Exception.hpp`](./src/Exception.hpp)  
现支持流式处理、强制处理、类型继承等功能
## 26v2a
- 拆分 TUI 头文件
- 拆出 [`StrUtils.hpp`](./src/Utility/StrUtils.hpp)  

<!--记得改徽章的版本！-->