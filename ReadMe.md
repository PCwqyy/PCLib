# PClib

<div style="text-align: center;">

[中文](ReadMe.md) | [English](ReadMe-EN.md)
</div>

![lang](https://img.shields.io/badge/标准-C++23-yellow?logo=cplusplus)
![version](https://img.shields.io/badge/版本-25v12c-blueviolet)
[![github](https://img.shields.io/badge/Github-PClib-blue?&logo=github
)](https://github.com/PCwqyy/PCLib)
[![gitee](https://img.shields.io/badge/Gitee-PClib-red?logo=gitee&color=%23C71D23
)](https://gitee.com/pcwqyy/PClib)


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

# PClib 1.2
- TUI 库！

# 更新日志
[历史](https://github.com/PCwqyy/PCLib/tree/Dev/ChangeLogHistory.md)

## 25v8a
### 新文件：'TUI.hpp'
- 添加了类 `TextBox` ：在屏幕上绘制文本框
### 文件：'Ansi.hpp'
- 稍微改了一下 `TUI.hpp`
## 25v9a
### 文件：'TUI.hpp'
- 添加了 **StyleSheet** 语法来控制文本框的样式
### 文件：'Color.hpp'
- 删除了颜色宏，改用 `NamedColor[]`
## 25v10a
- 将 `TUI.hpp` 文件拆分为 `src/TUI/` 下的多个文件
- 添加了 `ConsoleContext`
## 25v11a
### TUI
- 添加了 `ProgressBar'
- 修复了 `TextBox` 的逻辑错误
### 文件： 'File.hpp'
- 添加了 C++23 风格的输出
## 25v12a
### TUI
- 完成了 `ProgressBar`
~~TODO： 'AnsiString'~~ 换成了 'VisibleLen()'
## 25v12b
- 修复了逻辑错误
## 25v12c
- 修复了逻辑错误
- 为 `TextBox` 和 `ProgressBar` 添加了更多样式
## 25v13a
### TUI
- 向 `Element` 添加了类（class）系统现在，元素的样式将基于 `Element.eleStyle` （具有最高优先级） 和每个类的样式进行计算
## 25v14a
- 将 `ConsoleContext` 重命名为 `Document`
- 优化了项目结构
- 修复了拼写错误
- 修复了 'ReadMe.md' 中的语法错误
## PClib 1.2
- TUI库！