#flutter_cpp_physics_engine

A lightweight and efficient 2D physics engine written in C++, designed for integration with Flutter via FFI (Foreign Function Interface).

---

##Features:

- Custom 2D Vector2 math (position, velocity, scaling)
- Gravity and delta-time integration
- C++ performance with Dart FFI binding
- Easily embeddable in Flutter projects

---

##Folder Structure:
```
flutter_cpp_physics_engine/
├── src/         # C++ source code (multiple .cpp files)
├── include/     # C++ headers (optional)
├── flutter_example/  # Flutter project using this engine via FFI
├── .gitignore
├── CMakeLists.txt
├── LICENSE
└── README.md
```
---

##Installation:

Clone the repository:
```
git clone https://github.com/Khamit/flutter_cpp_physics_engine.git
cd flutter_cpp_physics_engine
```
---

##Build Instructions (C++):

Make sure you have CMake and g++/clang++ installed.

Run:
```
mkdir build
cd build
cmake ..
make
```
This will build the shared library:
```
- libgame_engine.so (Linux/Android)
- game_engine.dll (Windows)
- libgame_engine.dylib (macOS)
```
---

Flutter Integration:

To use the native engine in your Flutter app:
```
1. Copy the C++ source files:

   Create a folder named "cpp" inside your Android app source directory:

   android/app/src/main/cpp/

   Copy all the files from the "src" directory of this repo, plus the CMakeLists.txt file, into this new "cpp" folder.

2. Configure Android build:

   Make sure your app's build.gradle and CMakeLists.txt are configured to build the native library from this cpp folder.

3. Add the ffi package to your pubspec.yaml:

   dependencies:
     ffi: ^2.0.1

4. Load the native library in Dart code (example):

   import 'dart:ffi';

   final dylib = DynamicLibrary.open('libgame_engine.so'); // Android/Linux

5. Bind native functions with dart:ffi as demonstrated in the flutter_example directory.
```
---

License:

This project is licensed under the MIT License. See the LICENSE file for details.
