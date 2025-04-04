## 25v1a
### `Console.hpp`
- Split `printf` series into Normal / Color / Unicode parts.  
Enable them by define `PCL_COLOR` and `UNICODE`.
- Add environmental check
### `Color.hpp`
- Add HTML named colors
- Add color calcs, such as `RGB()`, `HLS()`...
### `Log.hpp`
- Fix misspelling.
## 25v2a
### New File: `Socket.hpp`
- Packaged up WinSock.
## 25v3a
### New Demo: `CMD Fight`
A game about attacking other using cmd and defending with regex.
### `Socket.hpp`
- Fix packet splicing
### `Console.hpp`
- New class `ConCursor`: provides multi-cursor for console.
## 25v3b
- fix bugs for demo `CMD Fight`
- Complete `ReadMe.md`
## 25v3c
- Slightly `CMD Fight` update
## 1.1.8
- Change C++ standard into c++23, and change (my) compiler into MinGW.
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

|Format|Escape|Description|
|-:|:-|:-|
|End region|`$/`|End latest trigger's effect|
|Bold|`$b`|**Bold**|
|Color 8|`$q(b\|f)`|4-bit color, `f` means foreground, and `b` meas back.<br>Ex. `qb3` means set background color to yellow.
|Color 256|`$c(q\|f)`|8-bit color|
|Darken|`$d`|Black text|
|Grey|`$g`|Gray text|
|Invert|`$!`|Invert color|
|Italic|`$i`|*Italic*|
|Link|`$l[url](text)`|[text](url)|
|StrickThrough|`$s`|~~StrickThrough~~|
|Twinkle|`$t`|Blinking text|
|Underline|`$u`|Underline|
## 25v7b
### File: `Ansi.hpp`, `Color.hpp`
Support true color ANSI out.

|Format|Escape|Description|
|-:|:-|:-|
|COLOR_256|`$c(q\|f)[color]`|true color, `color` accept HTML named colors (`DodgerBlue`, Case sensitive) and HEX colors (`#abcdef`)|
## 25v7c
### File: `Ansi.hpp`
- Complete `Ansi.hpp`.
- link syntax changed into `$l[text](url)`. ~~More reasonable, didn't it?~~
## 1.1.9
- Command managing
- More powerful socket operating
- Ansi print