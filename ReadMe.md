# PClib

![lang](https://img.shields.io/badge/Standard-C++23-yellow?logo=cplusplus)
![version](https://img.shields.io/badge/Version-25v9a-blueviolet)
[![github](https://img.shields.io/badge/Github-PClib-blue?&logo=github
)
](https://github.com/PCwqyy/PCLib)
[![gitee](https://img.shields.io/badge/Gitee-PClib-red?logo=gitee&color=%23C71D23
)](https://gitee.com/pcwqyy/PClib)

This is a personal C++ lib, and some features only available on Windows.

It includes:
- Text user interface
- Packaged containers
- IO & file operations
- Ansi escaping
- Some algorithms
- Math supporting
- Color calculation
- (Windows) Web socket
- (Windows) Console extension
- And more...

## Contact

The author is a Chinese high school student, who is not so good at English, so please forgive him when you found that he made some stupid mistakes (commonly misspellings).

E-Mail: w123567910@outlook.com  
Blogs: [www.cnblogs.com/PCwqyy](https://www.cnblogs.com/PCwqyy)

# PClib 1.2 (Not Released)
- TUI lib! Try to become a C++ version [Rich](https://github.com/Textualize/rich).

# Change Log
[History](https://github.com/PCwqyy/PCLib/tree/Dev/ChangeLogHistory.md)

## 25v8a
### New File: `TUI.hpp`
- Add class `TextBox` to draw a text box on the screen.
### File: `Ansi.hpp`
- Slightly update for `TUI.hpp`.
## 25v9a
### File: `TUI.hpp`
- Add **StyleSheet** syntax to control the style of text box.
### File: `Color.hpp`
- Remove color macros. Use `NamedColor[]` instead.
## 25v10a
- Split file `TUI.hpp` to multi files under `src/TUI/`.
- Add console context.
## 25v11a
### TUI
- Add `ProgressBar`.
- Fixed `TextBox` logic mistakes.
### File: `File.hpp`
- Add c++23 style out.
## 25v12a
### TUI
- `ProgressBar` finished
TODO: `AnsiString`
## 25v12b
- Fix logic mistakes
## 25v12c
- Fix logic mistakes
- Add more styles for `TextBox` & `ProgressBar`