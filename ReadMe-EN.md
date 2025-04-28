# PClib

<div style="text-align: center;">

[English](ReadMe-EN.md) | [中文](ReadMe.md) 

![lang](https://img.shields.io/badge/Standard-C++23-yellow?logo=cplusplus) ![version](https://img.shields.io/badge/Version-25v16b-blueviolet) [![github](https://img.shields.io/badge/Github-PClib-blue?&logo=github)](https://github.com/PCwqyy/PCLib) [![gitee](https://img.shields.io/badge/Gitee-PClib-red?logo=gitee&color=%23C71D23)](https://gitee.com/pcwqyy/PClib)

</div>

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
Simply clone the repo and include header files.

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

~~Maybe native speakers will think my tone is too formal. If so, I feel pretty sorry about this. Our high school in China just teaches English like this.~~

Email: w123567910@outlook.com  
Blogs: [www.cnblogs.com/PCwqyy](https://www.cnblogs.com/PCwqyy)

## License
The project uses [MIT License](https://opensource.org/licenses/MIT).

## Contributing
Any contribution is welcome! You can take part through the methods below:
1. Submit issues to report a bug or suggest a new feature
2. Submit pull requests to fix issues
3. Improve documents

# PClib 1.2
- TUI lib!

# Change Log
[History](ChangeLogHistory-EN.md)

## 25v15a
- Added some comment (Doc actually)
- Optimized `Large`, and it temporarily becomes unsigned...
## 25v15b
- Make `Large` sign again ~~(MLSA)~~
## 25v15c
- Added class `pc::Exception`, which will be used in exception management.
- Refined throws in `Large`
## 25v15d
- Comments and spellings.
## 25v16a
- Began to write DOM tree
## 25v16b
- Began to write PCML parser
- Considering rename the TUI part to *钼 Molybdenum*?
## 25v16c
- PCML element parser finished, but haven't debugged yet.