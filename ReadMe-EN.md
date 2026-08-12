# PClib

<div style="text-align: center;">

[English](ReadMe-EN.md) | [中文](ReadMe.md) 

<!-- 快照 blueviolet，正式版 green -->
![lang](https://img.shields.io/badge/Standard-C++23-yellow?logo=cplusplus)
![version](https://img.shields.io/badge/Version-26v1a-blueviolet)
[![github](https://img.shields.io/badge/Github-PClib-blue?&logo=github)](https://github.com/PCwqyy/PCLib)
[![gitee](https://img.shields.io/badge/Gitee-PClib-red?logo=gitee&color=%23C71D23)](https://gitee.com/pcwqyy/PClib)

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

## Demonstrantions
See [here](./demo/).

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

## Version Numbering
So, how the hell did version numbering work?

- For **release versions**:
  - Use the format `1.X.Y`
  - `X` = major version number
  - `Y` = minor version number; if `Y` is 0, it is omitted
  - Example: `1.2` `1.1.7`

- For **snapshot versions**:
  - Use the format `YYvZZX` ~~inspired by Mojang's scheme~~
  - `YY` = year (two digits)
  - `ZZ` = major snapshot number within the year
  - `X` = minor snapshot number within the major snapshot
    - Assigned alphabetically (a, b, c, ...)
    - If the snapshot is broken or non‑runnable, mark it with `q`, `qq`, `q3`, `q4`, ... (such snapshots usually do not include a README)
  - Examples: `24v3a`, `25v15e`, `26v1q`, `24v5qq`, `24v5q5`

# PClib 1.2.1 (Not released)
- DOM structure for TUI lib. (This is not a small change)
- Auto syncing data container `SyncedData`

# Change Log
[History](ChangeLogHistory-EN.md)

## 26v1a
- Significantly revised [`Exception.hpp`](./src/Exception.hpp)  
Now supports streaming, mandatory handling, type inheritance, and more.

<!--记得改徽章的版本！-->