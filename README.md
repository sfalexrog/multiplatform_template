# multiplatform template,

a template project for cross-platform, GPU-enabled software.

## Why, though?

I've done several OpenGL-based projects (some of which I wish to rewrite and publish), and they all had basically the same structure: [SDL2](https://libsdl.org), [dear imgui](https://github.com/ocornut/imgui), ran on Windows/Linux (even Android sometimes). A lot of time was spent writing the same boilerplate code over and over again, tweaking the same things, fighting the same (buildsystem) bugs again and again. This template should (eventually) be a starting point for writing GPU apps without worrying about portability.

## Okay, so what are the goals here?

At the very least, I'm targeting these platforms (in no particular order):

- [x] GNU/Linux
- [x] Web (through Emscripten)
- [x] Android
- [ ] Windows
- [ ] macOS
- [ ] iOS

Ideally, each resulting project should have a web version that is automatically deployed to Github Pages. This means using a CI system (for which a template is also a welcome thing).

Each platform is going to present its own set of challenges. These challenges will eventually be documented in a separate file. (**UPDATE**: [some info there](docs/platform-quirks.md))
