# PClib

This is a personal C++ lib, and some features only available on Windows.

It includes:
- Packaged containers
- IO & file operations
- Math supporting
- Color calculation
- And more...

The author is a Chinese high school student, who is not so good at English, so please forgive him when you found that he made some stupid mistakes.

View it on [Github](https://github.com/PCwqyy/PCLib).

# PClib 1.1.8 (Not released)
- Emojis for console!
- Color calculations!
- and more!


# Change Log
## 25v1a
### `Console.hpp`
- Split `printf` series into Normal / Color / Unicode parts.  
Enable them by define `PCL_COLOR` and `UNICODE`.
- Add environmental check
### `Color.hpp`
- Add HTLM named colors
- Add color calcs, such as `RGB()`, `HLS()`...
### `Log.hpp`
- Fix misspelling.
## 25v2a
### New File: `Socket.hpp`
- Packaged up WinSock.
