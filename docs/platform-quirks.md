# Platform quirks

Not every platform is created equal, and this is the place where inequalities are documented. This may help creating the build environment for each platform, so here goes.

## (Ubuntu) Linux

(Un)surprisingly enough, this one was the easiest to get up and running. Obviously there are some packages that are required to build and run the app.

If you're using system SDL2, run the following command:

```bash
sudo apt-get install build-essential cmake make libsdl2-dev libegl1-mesa-dev libgles2-mesa-dev
```

If, on the other hand, you wish to build SDL2 from sources, run:

```bash
sudo apt-get install build-essential cmake mercurial make autoconf automake libtool libasound2-dev libpulse-dev libaudio-dev libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev libxxf86vm-dev libxss-dev libgl1-mesa-dev libesd0-dev libsdl2-dev libegl1-mesa-dev
```

## Emscripten

This platform is relatively painless... until you consider how many browsers are there in the wild and how terrible their webgl2 support is (looking at you, Safari). Still, building the project is relatively painless, you can use the provided Docker file to create the build environment (run `docker/build.sh`) and build the project (run `docker/emcompile.sh`).

The main inconvenience comes from the fact that the `.html` file is not created by default. What's worse is that you can't really just set the default target suffix to `.html`, this will break CMake configuration step. So you'll have to add this suffix on a per-target basis; in `src/CMakeLists.txt` you can see the following:

```cmake
if(EMSCRIPTEN)
    message(STATUS "Building for Emscripten")
    set_target_properties(main PROPERTIES SUFFIX ".html")
endif()
```

This adds the required suffix only to the `main` target, which is your executable.

## Android

> Update (2020.03.18): SDL2 2.0.12 actually seems to fix the problems that are outlined below, making Android a nicely supported target!

Funny how older versions of SDL2 for Android were extremely easy to build, isn't it? Well, not anymore, and it's not even the fault of the SDL2 developers.

See, SDL2 uses `dlopen()` and `dlfcn()` calls to find your `main` function (which is transformed to `SDL_main` by the magic of macros). Now, there's nothing inherently bad with that, the only loaded libraries are the ones in your package, but somehow `dlopen()` seems to fail, and your `dlerror()` call (that you perform right after `dlopen()`, of course) returns `NULL`. So what's the deal here?

Well, it all starts with CMake. Google specifically tells the developers to [use the `find_library()`](https://developer.android.com/studio/projects/configure-cmake#add-ndk-api) CMake call to find the correct NDK APIs, and yet [`find_library(dl-lib dl)` falls on its face](https://github.com/android/ndk/issues/929) by finding the static `libdl.a` stub instead of the proper dynamic `libdl.so`. (At the time of this writing, the issue was still not fixed, by the way. Guess it's Google's policy to keep the NDK team understaffed?)

So what do we do about that? Well, we nudge CMake in the right direction by finding the proper library ourselves:

```cmake
if (ANDROID)
    message(STATUS "Android: Using a workaround for libdl.so linkage")
    find_library(ANDROID_DL_LIBRARY NAMES libdl.so dl)
    message(STATUS "ANDROID_DL_LIBRARY set to ${ANDROID_DL_LIBRARY}, hope this will convince SDL2")
endif()
add_subdirectory(SDL2)
```

This "fix" relies on `ANDROID_DL_LIBRARY` being used in SDL2 configuration, and will surely break at some point in time.

Another fun thing is the `hidapi` target. It is not linked to any SDL2 target by default, and omitting it results in build failures. On the other hand, this target is Android-specific. A kinda-reasonable solution would be to create an interface target that links to both SDL2 and hidapi, alias it as `SDL2::SDL2`, and use that.

## Windows

TBD (the short version is: you'll need to specify output directories, you'll need to use something like [ANGLE](https://chromium.googlesource.com/angle/angle), and there's probably something else that I don't know)

## macOS

TBD (shouldn't be too bad, right? It's just Unix, after all... except that there will probably be a ton of issues with OpenGL *and* OpenGL ES)

## iOS

TBD (my guess is that iOS won't be supported in CMake properly, and you *have* to use XCode, so I expect this platform to be the largest source of pain)
