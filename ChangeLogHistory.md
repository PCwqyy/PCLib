## 25v8a
### 新文件：`TUI.hpp`
- 添加了类 `TextBox` ：在屏幕上绘制文本框
### 文件：`Ansi.hpp`
- 稍微改了一下以适配 `TUI.hpp`
## 25v9a
### 文件：`TUI.hpp`
- 添加了 **StyleSheet** 语法来控制文本框的样式
### 文件：`Color.hpp`
- 删除了颜色宏，改用 `NamedColor[]`
## 25v10a
- 将 `TUI.hpp` 文件拆分为 `src/TUI/` 下的多个文件
- 添加了 `ConsoleContext`
## 25v11a
### TUI
- 添加了 `ProgressBar`
- 修复了 `TextBox` 的逻辑错误
### 文件： `File.hpp`
- 添加了 C++23 风格的输出
## 25v12a
### TUI
- 完成了 `ProgressBar`
~~TODO： `AnsiString`~~ 换成了 `VisibleLen()`
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
- 修复了 `ReadMe.md` 中的语法错误
## PClib 1.2
- TUI库！