# flutter_cpp_physics_engine

A lightweight and efficient 2D physics engine written in C++, designed for integration with Flutter via FFI (Foreign Function Interface).

---

## 🚀 Features

- Custom 2D `Vector2` math (position, velocity, scaling)
- Gravity and delta-time integration
- C++ performance with Dart FFI binding
- Easily embeddable in Flutter projects

---

## 📁 Folder Structure

```
flutter_cpp_physics_engine/
├── src/ # C++ source code
├── include/ # C++ headers (optional)
├── flutter_example/ # Flutter project using this engine via FFI
├── .gitignore
├── CMakeLists.txt
├── LICENSE
└── README.md
```
---

## 📦 Installation

Clone the repo:

```bash
git clone https://github.com/Khamit/flutter_cpp_physics_engine.git
cd flutter_cpp_physics_engine
```
---


## 🔧 Build Instructions (C++)
Make sure you have CMake and g++/clang++ installed.

Run:

```bash
mkdir build
cd build
cmake ..
make
```
This will build your shared library:

libgame_engine.so (Linux/Android)

game_engine.dll (Windows)

libgame_engine.dylib (macOS)

---

## 🔗 Flutter FFI Integration (Basic)
In your Flutter app:

1. Add ffi to pubspec.yaml
```
dependencies:
  ffi: ^2.0.1
```
2. Load the native library in Dart
```
final dylib = DynamicLibrary.open('libgame_engine.so'); // Android/Linux
Bind to native methods using dart:ffi.
```
3. Bind C++ methods with dart:ffi
Refer to the flutter_example/ directory for full integration (coming soon).
```
import 'dart:ffi';

final dylib = DynamicLibrary.open('libgame_engine.so'); // Adjust per platform
```

## 📜 License
This project is licensed under the MIT License. See the LICENSE file for details.