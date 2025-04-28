# PClib

<div style="text-align: center;">

[中文](ReadMe.md) | [English](ReadMe-EN.md)

![lang](https://img.shields.io/badge/标准-C++23-yellow?logo=cplusplus) ![version](https://img.shields.io/badge/版本-25v16d-blueviolet) [![github](https://img.shields.io/badge/Github-PClib-blue?&logo=github)](https://github.com/PCwqyy/PCLib) [![gitee](https://img.shields.io/badge/Gitee-PClib-red?logo=gitee&color=%23C71D23)](https://gitee.com/pcwqyy/PClib)

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

## 25v15a
- 写了一些注释文档
- 优化了 `Large`，但是暂时变成 unsigned 的了……
## 25v15b
- 重新添加了 `Large` 的符号
## 25v15c
- 添加了 `pc::Exception` 类，用于管理异常
- 完善了 `Large` 的异常处理
## 25v15d
- 注释和拼写
## 25v16a
- 开始写 DOM 树
## 25v16b
- 开始写 PCML 解析器
- 考虑把 TUI 改名为 *钼 Molybdenum*？
## 25v16c
- 元素 PCML 解析器完成，尚未调试
## 25v16d
- `Element::QuerySelectorAll()` 的标签、类、ID选择器完成，后代选择器完成，差一个子元素选择器
- 修复了 `pc::File::String()` 总会多一个 `\377` 的问题

<!--记得改徽章的版本！-->