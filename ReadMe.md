# PClib

![lang](https://img.shields.io/badge/Standard-C++23-yellow?logo=cplusplus)
![version](https://img.shields.io/badge/Version-25v12c-blueviolet)
[![github](https://img.shields.io/badge/Github-PClib-blue?&logo=github
)](https://github.com/PCwqyy/PCLib)
[![gitee](https://img.shields.io/badge/Gitee-PClib-red?logo=gitee&color=%23C71D23
)](https://gitee.com/pcwqyy/PClib)

This is a personal C++ lib, and some features are only available on Windows.

## Features
- Text user interface
- Packaged containers
- IO & file operations
- Ansi escaping
- Some algorithms
- Math support
- Color calculation
- (Windows) Web socket
- (Windows) Console extension
- And more...

## Install
Simply clone the repo and include the header file.

```bash
git clone https://github.com/PCwqyy/PCLib.git
git clone https://gitee.com/pcwqyy/PClib.git
```

Example:

```cpp
#include"/path/to/PCLib/TUI/TUI.hpp"
```

## Contact
The author is a Chinese high school student who is not so good at English, so please forgive him when you find that he made some stupid mistakes (commonly misspellings).

Email: w123567910@outlook.com  
Blogs: [www.cnblogs.com/PCwqyy](https://www.cnblogs.com/PCwqyy)

## License
The project uses [MIT License](https://opensource.org/licenses/MIT). Free  of charge, hereby granted.

## Contributing
Any contribution is welcome! You can take part through the methods below:
1. Submit issues to report a bug or suggest a new feature
2. Submit pull requests to fix issues
3. Improve documents.

# PClib 1.2 (Not Released)
- TUI lib! Trying to become a C++ version of [Rich](https://github.com/Textualize/rich).

# Change Log
[History](https://github.com/PCwqyy/PCLib/tree/Dev/ChangeLogHistory.md)

## 25v8a
### New File: `TUI.hpp`
- Added class `TextBox` to draw a text box on the screen.
### File: `Ansi.hpp`
- Slightly updated for `TUI.hpp`.
## 25v9a
### File: `TUI.hpp`
- Added **StyleSheet** syntax to control the style of text box.
### File: `Color.hpp`
- Removed color macros. Use `NamedColor[]` instead.
## 25v10a
- Split file `TUI.hpp` into multiple files under `src/TUI/`.
- Added `ConsoleContext`.
## 25v11a
### TUI
- Add `ProgressBar`.
- Fixed `TextBox` logic mistakes.
### File: `File.hpp`
- Added C++23 style output.
## 25v12a
### TUI
- `ProgressBar` finished
~~TODO: `AnsiString`~~ Use `VisibleLen()` instead.
## 25v12b
- Fixed logic mistakes
## 25v12c
- Fixed logic mistakes
- Added more styles for `TextBox` & `ProgressBar`
## 25v13a
### TUI
- Added class system to `Element`. Now elements' style will be calc with `Element.eleStyle` (which has the highest priority) and the style of each class.
## 25v14a
- Renamed `ConsoleContext` to `Document`
- Optimized project structure.
- Fixed misspellings.
- Fixed grammar errors in `ReadMe.md`.