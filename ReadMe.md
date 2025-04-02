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

View it on [Github](https://github.com/PCwqyy/PCLib) or [Gitee](https://gitee.com/pcwqyy/PClib).

# PClib 1.1.9 (Not released)
- Command managing
- More powerful socket operating
- Ansi output

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
- ~~TODO: Check out `std::format`~~ Done.
## 25v6b
### File: `Socket.hpp`
- Now `ClientSocket::Connect()` accepts address.
## 25v7a
### New File: `Ansi.hpp`
- Provide functions (`AnsiPrase()`) that escapes string to [ANSI](https://invisible-island.net/xterm/ctlseqs/ctlseqs.html) string which contains formatting information that used in virtual console.

|Format|Escape|Discription|
|-:|:-|:-|
|END_REGION|`$/`|End latest trigger's effect|
|BOLD|`$b`|**Bold**|
|COLOR_8|`$q(b\|f)`|4-bit color, `f` means foreground, and `b` meas back.<br>Ex. `qb3` means set background color to yellow.
|COLOR_256|`$c(q\|f)`|8-bit color|
|DARKEN|`$d`|Black text|
|GRAY|`$g`|Gray text|
|INVERT|`$!`|Invert color|
|ITALIC|`$i`|*Italic*|
|LINK|`$l[url](dis)`|[dis](url)|
|STRIKETHROUGH|`$s`|~~StrickThrough~~|
|TWINKLE|`$t`|Blinking text|
|UNDERLINE|`$u`|Underline|