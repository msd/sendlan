# Requirements

* C++ compiler
* Conan
* Cmake 

# Deault profile

If you don't already have a conan profile, create a default profile:
`conan profile new --detect default`

# Install dependencies

```
cd build
conan install ../src --build=missing -pr=default
```

# Configure

```
cmake ../src -D CMAKE_TOOLCHAIN_FILE=conan_paths.cmake
```

# Compile

`cmake --build .`
