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
---
## Flutter steps
# Step 1: Structure your Flutter project to include native C++
Create your Flutter project (if not already done):
```
flutter create my_flutter_game
```
# 2.Enable Android NDK support:
Open android/app/build.gradle and add:
```
android {
    ...
    defaultConfig {
        ...
        externalNativeBuild {
            cmake {
                cppFlags ""
            }
        }
        ndk {
            abiFilters "armeabi-v7a", "arm64-v8a", "x86_64"
        }
    }

    externalNativeBuild {
        cmake {
            path "src/main/cpp/CMakeLists.txt"
        }
    }
}
```
# Step 2: Add C++ engine
 Create a folder: android/app/src/main/cpp/
 Add your game_engine.cpp file there.
 Add a header file if needed (game_engine.h).
 Create CMakeLists.txt in the same folder:
```
cmake_minimum_required(VERSION 3.4.1)

add_library(game_engine SHARED
    game_engine.cpp
)

find_library(log-lib log)

target_link_libraries(game_engine
    ${log-lib}
)
```
# Step 3: Create JNI Bridge
Create a file in android/app/src/main/cpp/bridge.cpp:
```
#include <jni.h>
#include "game_engine.cpp"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_myfluttergame_GameEngine_createBody(JNIEnv *env, jobject, float posX, float posY, float velX, float velY, float gravX, float gravY, float mass) {
    return reinterpret_cast<jlong>(create_body(posX, posY, velX, velY, gravX, gravY, mass));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myfluttergame_GameEngine_updateBody(JNIEnv *env, jobject, jlong ptr, float dt) {
    update_body(reinterpret_cast<PhysicsBody*>(ptr), dt);
}

extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_example_myfluttergame_GameEngine_getBodyPosition(JNIEnv *env, jobject, jlong ptr) {
    float x, y;
    get_body_position(reinterpret_cast<PhysicsBody*>(ptr), &x, &y);
    jfloatArray result = env->NewFloatArray(2);
    float temp[2] = {x, y};
    env->SetFloatArrayRegion(result, 0, 2, temp);
    return result;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myfluttergame_GameEngine_destroyBody(JNIEnv *env, jobject, jlong ptr) {
    destroy_body(reinterpret_cast<PhysicsBody*>(ptr));
}

```
# Step 4: Java/Kotlin wrapper
Create GameEngine.java in android/app/src/main/java/com/example/myfluttergame/:
```
package com.example.myfluttergame;

public class GameEngine {
    static {
        System.loadLibrary("game_engine");
    }

    public native long createBody(float posX, float posY, float velX, float velY, float gravX, float gravY, float mass);
    public native void updateBody(long bodyPtr, float dt);
    public native float[] getBodyPosition(long bodyPtr);
    public native void destroyBody(long bodyPtr);
}

```
# Step 5: Use in Dart (Flutter)
Use Platform Channels to talk from Dart to Java:
```
import 'package:flutter/services.dart';

class PhysicsController {
  static const _channel = MethodChannel('game_engine');

  Future<int> createBody(...) async {
    final result = await _channel.invokeMethod('createBody', {...});
    return result;
  }

  // Similarly for updateBody, getBodyPosition, destroyBody
}

```
# In MainActivity.kt:
```
class MainActivity: FlutterActivity() {
    private val engine = GameEngine()

    override fun configureFlutterEngine(flutterEngine: FlutterEngine) {
        super.configureFlutterEngine(flutterEngine)

        MethodChannel(flutterEngine.dartExecutor.binaryMessenger, "game_engine").setMethodCallHandler {
            call, result ->
            when (call.method) {
                "createBody" -> {
                    val args = call.arguments as Map<String, Double>
                    val ptr = engine.createBody(
                        args["posX"]!!.toFloat(),
                        args["posY"]!!.toFloat(),
                        args["velX"]!!.toFloat(),
                        args["velY"]!!.toFloat(),
                        args["gravX"]!!.toFloat(),
                        args["gravY"]!!.toFloat(),
                        args["mass"]!!.toFloat(),
                    )
                    result.success(ptr.toInt()) // Or Long if needed
                }
                // handle other methods similarly...
                else -> result.notImplemented()
            }
        }
    }
}

```
---
## 📜 License
This project is licensed under the MIT License. See the LICENSE file for details.
