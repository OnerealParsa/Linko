# Linko — Console Social Network (C++)

Linko is a text-based social network built as a console application in C++, focused on clean object-oriented design: encapsulation, composition and file-based data persistence.

## Features

- Register / login with username and password
- Post creation with auto-incrementing IDs
- Likes and comments on posts
- Follow system with per-user followers/following lists
- Feed sorting by time or by likes
- Persistent storage in `linko_data.txt` using a `|`-delimited serialization format

## Architecture

| File | Responsibility |
|---|---|
| `Models.h` | `User`, `Post` and `Comment` classes with serialization methods |
| `LinkoNetwork.h` | Core controller: authentication, posts, likes, comments and follow logic |
| `Date.h` | Timestamp handling and string (de)serialization |
| `main.cpp` | Command-line interface and quoted-string parsing |

## Design Notes

- Collections are never exposed directly — all mutations go through `LinkoNetwork` methods (encapsulation)
- The `|` character inside user text is escaped so the data format never breaks
- Quoted strings (`"..."`) are parsed as single arguments, so bios and posts can contain spaces

## Build & Run

```bash
g++ -std=c++17 -O2 -o linko main.cpp
./linko
```
