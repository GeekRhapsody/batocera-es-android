# Android Build Guide

This document describes how to build EmulationStation for Android (first-pass: get the app launching).

## Prerequisites

| Tool | Version | Notes |
|---|---|---|
| Android Studio | Hedgehog (2023.1+) or newer | Or command-line tools |
| Android NDK | r25c / r26b | Install via SDK Manager → NDK (Side by side) |
| CMake | 3.22.1 | Install via SDK Manager → CMake |
| vcpkg | latest | For C++ dependencies |
| JDK | 17 | Bundled with Android Studio |

## Step 1 — Clone with submodules

```sh
git clone --recurse-submodules <repo-url>
# or, if already cloned:
git submodule update --init
```

This pulls SDL2 into `external/SDL2` (the SDL2 branch of libsdl-org/SDL).

## Step 2 — Install C++ dependencies with vcpkg (Android triplet)

The NDK build needs prebuilt ARM64 versions of FreeType, FreeImage, cURL, RapidJSON, and SDL2_mixer.

```sh
# Clone vcpkg if you don't have it
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh   # (Linux/macOS) or bootstrap-vcpkg.bat (Windows)

# Set ANDROID_NDK_HOME before installing
export ANDROID_NDK_HOME=/path/to/ndk  # e.g. ~/Android/Sdk/ndk/26.1.10909125

# Install dependencies for arm64-android
./vcpkg/vcpkg install \
    freetype:arm64-android \
    freeimage:arm64-android \
    curl[core,openssl]:arm64-android \
    sdl2-mixer:arm64-android \
    rapidjson:arm64-android

# Note: SDL2 itself is built from source (external/SDL2), not via vcpkg.
```

> **If FreeImage fails**: it is the hardest dependency. See the "FreeImage fallback" section below.

## Step 3 — Configure android/app/build.gradle

Edit `android/app/build.gradle` and add your vcpkg toolchain path to the CMake arguments:

```gradle
externalNativeBuild {
    cmake {
        arguments '-DGLES2=ON',
                  '-DCEC=OFF',
                  // ... other args ...
                  '-DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake',
                  '-DVCPKG_TARGET_TRIPLET=arm64-android',
                  '-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake'
        abiFilters 'arm64-v8a'
    }
}
```

## Step 4 — Build

### Via Android Studio
1. Open the `android/` directory as an Android Studio project
2. Let Gradle sync
3. **Build → Make Project** or **Run → Run 'app'**

### Via command line
```sh
cd android
./gradlew assembleDebug
# APK lands at: android/app/build/outputs/apk/debug/app-debug.apk
```

## Step 5 — Install and run

```sh
adb install android/app/build/outputs/apk/debug/app-debug.apk
adb shell am start -n org.es.android/.ESActivity
```

Check logcat for output:
```sh
adb logcat -s SDL,EmulationStation
```

## Expected first-run behaviour

- The app launches and shows EmulationStation's splash screen
- "No systems found" message appears (expected — no ROM paths configured yet)
- D-pad/buttons on a connected Bluetooth gamepad should navigate the UI

## ROM and config paths (Android)

| Purpose | Path |
|---|---|
| Config / logs | `/data/data/org.es.android/files/es/` (internal, private) |
| Themes | `<external>/themes/` |
| ROMs | `<external>/` (one folder per system, matching `es_systems.cfg`) |
| Screenshots | `<external>/screenshots/` |

`<external>` resolves to the app's external storage directory, typically:
`/sdcard/Android/data/org.es.android/files/`

## FreeImage fallback

If vcpkg cannot build FreeImage for Android, you can replace it with `stb_image`:

1. Add `stb_image.h` to `external/stb/`
2. Patch `es-core/src/ImageIO.cpp` and `es-core/src/resources/TextureData.cpp` to use `stb_image`
   instead of FreeImage calls
3. Remove FreeImage from `CMakeLists.txt` `find_package` and `COMMON_LIBRARIES`

This is a larger change tracked as a follow-up task.

## Current limitations (first pass)

- No video playback (LibVLC disabled; follow-up: LibVLC for Android or ExoPlayer)
- No emulator launching (requires Android Intent integration)
- No Bluetooth/WiFi management (Android system APIs not yet wired)
- No CEC, TTS, game controller autoconfiguration scripts
- ROM scanner works but paths must be manually configured in `es_systems.cfg`

## Iteration roadmap

1. First launch ← **you are here**
2. Theme loading from `assets/` APK bundle
3. Touch navigation
4. ROM scanning from external storage
5. Launch emulators via Android Intents
6. Video playback (LibVLC Android build)
7. Bluetooth gamepad autoconfiguration
