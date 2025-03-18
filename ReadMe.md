# PClib

This is a personal C++ lib, and some features only available on Windows.

It includes:
- Packaged containers
- IO & file operations
- Some algorithms
- Math supporting
- Color calculation
- Web socket
- Console extension
- And more...

The author is a Chinese high school student, who is not so good at English, so please forgive him when you found that he made some stupid mistakes (commonly misspellings).

View it on [Github](https://github.com/PCwqyy/PCLib).

# PClib 1.1.9 (Not released)
- Command managing
- More powerful socket operating

# Change Log
[History](https://github.com/PCwqyy/PCLib/tree/Dev/ChangeLogHistory.md)
## 25v4a
- Modified all file's header.
- Update demo `CmdFight` with `Command.hpp`
### New file: `Command.hpp`
- A easy way to explain a command line.
- Not finished yet.
### `Socket.hpp`
- Finished socket recycle part.
## 25v5a
### New file: `VarSet.hpp`
- Variant set of `std::any`
### `Command.hpp`
- Finished with `VarSet`
- Now the command func only accept those params are `VarSet`
- TODO: subcommand & trigger
### `IO.hpp`
- Finished input part ~~(the file before is called `O.hpp`)~~
- Fit `int`, `long long`, `double`, `std::string`, `char`, `char[]`, `std::any` (by using `ScanAnyIn<type>()`), `Large`, `rational`.
- ~~TODO: colorful print~~ Done.
## 25v6a
- add `Multinclude.hpp` to the end of all file.
### New File: `Multinclude.hpp`
- Some cross include stuff moved into this file
- Not necessary to include manually.
- Finished colorful print(`xxxPrint()` series).
- TODO: Wide char print
- TODO: Check out `std::format`;