# transferserver-qt

# A simple TCP server for file and message transfer

## Demo (video)


## Client
Use the client application to test the server:  
https://github.com/mxstnquu/transferclients-qt


## Features

- Receive text messages and broadcast them to all connected clients
- Receive files and save them to the `file_downloads/` directory
- Handle multiple clients
- Automatic file renaming when a duplicate name exists


## Dependencies

- Qt6 (Core, Network)
- CMake
- g++


## Build

```bash
git clone https://github.com/mxstnquu/transferserver-qt
cd ./transferserver-qt
cmake -S . -B build
cmake --build build
./build/transferserver-qt
```


## Project structure

transferserver-qt/
├── CMakeLists.txt
├── include/
│   ├── Server.hpp
│   └── FileHandler.hpp
├── src/
│   ├── Server.cpp
│   └── FileHandler.cpp
├── main.cpp
└── file_downloads/   (created automatically)
