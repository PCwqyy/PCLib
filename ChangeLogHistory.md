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