# Lab 1: Hash Function Implementations in C++

This repository contains the implementation of various hash functions in C++. The implemented algorithms include:

- MD5
- SHA1
- SHA256
- SHA512
- MurmurHash3
- CityHash
- FarmHash
- Whirlpool
- BLAKE2s

## Libraries Used

Make sure to use the following libraries:

- **OpenSSL** (for MD5, SHA1, SHA256, SHA512, Whirlpool, BLAKE2s)
- **CityHash**
- **FarmHash**

## Setup Instructions

### Installing CityHash

To install CityHash, follow these steps:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake
git clone https://github.com/google/cityhash.git
cd cityhash
./configure
make    
sudo make install
```

### Installing Farmhash
To install FarmHash, follow these steps:
```bash
git clone https://github.com/google/farmhash.git
cd farmhash
sudo apt-get install build-essential autoconf automake libtool
./configure
make
```
> [!CAUTION]
> If you encounter errors in the farmhash.cc file, do the following:

- Open the farmhash.cc file
- Modify the line 
  ```static char data[kDataSize];``` to ```static char farm_data[kDataSize];``` and replace `data` with `farm_data` wherever needed.

### Configuring tasks.json for build tasks
For MD5, SHA1, SHA256, SHA512 

Configure your tasks.json as follows:
```bash
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "shell",
            "label": "g++ build active file",
            "command": "/usr/bin/g++",
            "args": [
                "-g",
                "${file}",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}",
                "-lcrypto",
                "-std=c++11"
            ],
            "options": {
                "cwd": "/usr/bin"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

For CityHash

Use the following configuration for CityHash:
```bash
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "shell",
            "label": "g++ build active file",
            "command": "/usr/bin/g++",
            "args": [
                "-g",
                "-std=c++11",
                "-O2",
                "${file}",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}",
                "-lcityhash",
                "-I/usr/local/include",
                "-L/usr/local/lib"
            ],
            "options": {
                "cwd": "/usr/bin"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```
For FarmHash

To configure for FarmHash, simply change -lcityhash to -lfarmhash in the tasks.json:
```bash
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "shell",
            "label": "g++ build active file",
            "command": "/usr/bin/g++",
            "args": [
                "-g",
                "-std=c++11",
                "-O2",
                "${file}",
                "-o",
                "${fileDirname}/${fileBasenameNoExtension}",
                "-lfarmhash",
                "-I/usr/local/include",
                "-L/usr/local/lib"
            ],
            "options": {
                "cwd": "/usr/bin"
            },
            "problemMatcher": [
                "$gcc"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}

```