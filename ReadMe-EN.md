# PClib

<div style="text-align: center;">

[English](ReadMe-EN.md) | [中文](ReadMe.md) 

<!-- 快照 blueviolet，正式版 green -->
![lang](https://img.shields.io/badge/Standard-C++23-yellow?logo=cplusplus)
![version](https://img.shields.io/badge/Version-1.2.1-green)
[![github](https://img.shields.io/badge/Github-PClib-blue?&logo=github)](https://github.com/PCwqyy/PCLib)
[![gitee](https://img.shields.io/badge/Gitee-PClib-red?logo=gitee&color=%23C71D23)](https://gitee.com/pcwqyy/PClib)

</div>

This is a personal C++ lib, and some features are only available on Windows.

## Features
- Text User Interface
- Color computation
- IO & file operations
- ANSI escape
- Unicode handling
- _Math support_
- Container classes
- (Windows) Web sockets
- (Windows) Console extensions
- And more...

## Install
Simply clone the repo and include header files.

```bash
git clone https://github.com/PCwqyy/PCLib.git
git clone https://gitee.com/pcwqyy/PClib.git
```

Example:

```cpp
#include"/Utility/Ansi.hpp"
```

Please add `-I /path/to/pclib/src` into compiling arguments.

## Demonstrantions
See [here](./demo/).

## Contact
The author is a Chinese high school student who is not so good at English, so please forgive him when you find that he made some stupid mistakes (commonly misspellings).

~~Maybe I sound too formal to native speakers. If so, I apologize - it's just how we were taught English in Chinese high schools.~~

Email: w123567910@outlook.com  
Blogs: [www.cnblogs.com/PCwqyy](https://www.cnblogs.com/PCwqyy)

## License
The project uses [MIT License](https://opensource.org/licenses/MIT).

## Contributing
Any contribution is welcome! You can take part through the methods below:
1. Submit issues to report a bug or suggest a new feature
2. Submit pull requests to fix issues
3. Improve documents

## Version Numbering
So, how the hell did version numbering work?

- For **release versions**:
  - Use the format `1.X.Y`
  - `X` = major version number
  - `Y` = minor version number; if `Y` is 0, it is omitted
  - Example: `1.2` `1.1.7`

- For **pre-release versions**  
  Add `rcZ` after `1.X.Y`

- For **snapshot versions**:
  - Use the format `YYvZZX` ~~inspired by Mojang's scheme~~
  - `YY` = year (two digits)
  - `ZZ` = major snapshot number within the year
  - `X` = minor snapshot number within the major snapshot
    - Assigned alphabetically (a, b, c, ...)
    - If the snapshot is broken or non‑runnable, mark it with `q`, `qq`, `q3`, `q4`, ... (such snapshots usually do not include a README)
  - Examples: `24v3a`, `25v15e`, `26v1q`, `24v5qq`, `24v5q5`

# PClib 1.2.1
- Auto-synchronized data container `SyncedData`
- Tagged error handling `pc::Exception`
- Multiple file type support `pc::File`
- **Removed all relative `#include`s, switched to absolute path addressing**

### In development...
| Feature | Planned Version | Sublibrary |
|-|-|-|
| Reflection serializer | 1.2.2 | Experimental |
| Reflection JSON parser | 1.2.2 | Experimental |
| Clean up obsolete classes in the legacy library | 1.2.3 | Container |
| Harden legacy components | 1.2.3 | Container |
| Refactor the Command library with reference to Minecraft source code | 1.2.4 | Container |
| Modularize all libraries | 1.2.5 | |
| Refactor the TUI library into a DOM structure | 1.3 | TUI |
# Change Log
[History](ChangeLogHistory-EN.md)

## 26v1a
- Significantly revised [`Exception.hpp`](./src/Exception.hpp)  
Now supports streaming, mandatory handling, type inheritance, and more.
## 26v2a
- Split the TUI header files
- Extracted [`StrUtils.hpp`](./src/Utility/StrUtils.hpp)  
- Improved error handling in [`Color.hpp`](./src/Container/Color.hpp)  
- Modified [`Exception.hpp`](./src/Exception.hpp)  
  Now supports type inheritance, allowing multiple types to be caught simultaneously
### New Demo: [Color](./demo/Color/)
- Demonstrates how to use the `Color` class

## 26v3a
- Removed `Multinclude.hpp`
- Refactored [`File.hpp`](./src/Utility/File.hpp)
- Improved the constant lookup class [`EnumLookup`](./src/Utility/EnumLookup.hpp)

Currently, [`File.hpp`](./src/Utility/File.hpp) is untested. The TUI is temporarily broken due to changes in [`EnumLookup`](./src/Utility/EnumLookup.hpp) and will be fixed in the next snapshot.

## 26v4a
Well, I'm going to put off fixing the TUI library...
### Renamed `Unicode.hpp` → `Unicodes.hpp`
- Refactored [`Unicodes.hpp`](./src/Utility/Unicodes.hpp): added conversions between UTF-8, UTF-16, and UTF-32, compatibility with `std::format`, and resolved cross-platform output issues
- Planning to write [`Meta.hpp`](./src/Experimental/Meta.hpp), which will use the C++26 reflection library

## 26v5a
- **Removed `VarSet.hpp`, `Sortting.hpp`, `IO.hpp`, and the fragile containers in [`Containers.hpp`](./src/Container/Containers.hpp) that can be replaced by the standard library**
- **Removed `Window.hpp`, because I never want to deal with WinAPI again**
- Modified `Meta.hpp`
- Added [`demo/Demos`](./demo/Demos/) for lightweight demos
- [`File.hpp`](./src/Utility/File.hpp) has completed initial testing
- **Removed all relative `#include`s, switched to absolute path addressing**
# 1.2.1
Congratulations! 1.2.1 is finally released  
~~The first major release of my senior year of high school~~

<!--记得改徽章的版本！-->