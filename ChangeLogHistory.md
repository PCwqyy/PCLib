更老的历史详见[此处](ChangeLogHistory-EN.md)
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
# PClib 1.2
- TUI库！
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
## 25v16e
- 修复了 `PCML::Make()` 的严重逻辑错误
- 完成了 `Element::QuerySelectorAll()`
## 25v17a
- 添加了新的容器：`SyncedData<Tp>`，用于创建一个实时相互同步的数据组，即修改一个值时，组内的其他变量会同步修改
## 25v17b
- 将 `SyncedData` 改为双端树，现在可以安全地析构了
## 25v18a
- 为 `SyncedData` 添加了拷贝函数，现在可以安全地拷贝了
- 将 `Element::ID`、`Element::ClassList` 改为 `Element::Attribute` 的映射而不是独立的变量，以匹配 Web DOM 的行为
- 将 `Element::QuerySelectorAll` 的返回类型改为指针指向原元素，以模仿 Web DOM 的行为 
## 25v19a
- 回滚 `Command.hpp` 至 25v4a
- 改动 [Cmd Fight](./demo/CmdFight/readme.md) 使其又可以编译
## 25v20a
- ~~优化了~~ 重写了 `PCML.hpp`
- 修复了 `Element.hpp` 的内存泄漏
## 25v20b
- 添加部分注释
- 优化  `Element.hpp`
- 新增部分功能
## 25v20c
- 添加部分注释
- 新增部分功能
### 新 Demo： [PCML](./demo/PCML/readme.md)
演示如何使用 PCML 及 `Element.hpp` 中的类DOM操作
## 25v20d
- 新增部分功能
### 新 Demo： [Clock](./demo/Clock/readme.md)
演示如何使用 [`Ansi.hpp`](./src/Utility/Ansi.hpp)
## 25v21a
- 写 [`Buffer.hpp`](./src/TUI/Buffer.hpp)
## 25v21b
- 分离 [`Unicode.hpp`](./src/Utility/Unicode.hpp)
- 优化 `Buffer.hpp`
## 25v22a
- 小修小补
## 25v22b
### 新 Demo： [Large](./demo/Large/readme.md)
演示如何使用 `Large` 类