# bigger

[![Build Status](https://travis-ci.com/yuki-koyama/bigger.svg?branch=master)](https://travis-ci.com/yuki-koyama/bigger)

bigg (bgfx + imgui + glfw + glm) + utils

This library, named `bigger`, is a prototype-oriented middleware library for 3D interactive applications. Based on a library named `bigg`, which stands for `bgfx` + `imgui` + `glfw` + `glm`, this library adds some higher-level utilities (such as renderable primitive classes) to make the prototyping of lightweight and cross-platform apps even easier.

![](./docs/screen_shot.png)

## Languages

- C++17
- GLSL 1.30

## Dependencies

- bigg <https://github.com/JoshuaBrookover/bigg> [Unlicense]
  - bgfx.cmake <https://github.com/JoshuaBrookover/bgfx.cmake> [CC0 1.0 Universal]
    - bgfx <https://github.com/bkaradzic/bgfx> [BSD 2-Clause]
    - bimg <https://github.com/bkaradzic/bimg> [BSD 2-Clause]
    - bx <https://github.com/bkaradzic/bx> [BSD 2-Clause]
  - Dear ImGui <https://github.com/ocornut/imgui> [MIT]
  - GLFW <https://github.com/glfw/glfw> [Zlib]
  - GLM <https://github.com/g-truc/glm> [MIT]
- random-util <https://github.com/yuki-koyama/rand-util> [MIT]
- string-util <https://github.com/yuki-koyama/string-util> [MIT]
- tinyobjloader <https://github.com/syoyo/tinyobjloader> [MIT]

## Main Classes

- App __(needs to be overridden)__
- Camera
- Primitives
  - Cube primitive
  - Dynamic mesh primitive
  - Mesh primitive
  - Plane primitive
  - Sphere primitive
- Materials
  - Blinn-Phong material

## App Event Cycle

- `bigger::App::runApp()`
  - `bigg::Application::run()`
    - `glfw` initialization
    - `bgfx` initialization
    - `imgui` initialization
    - Reset
    - `bigger::App::initialize()` __(needs to be overridden)__
    - Main loop
      - `glfw` event polling
      - `imgui` event polling
      - `bigger::App::update()`
        - `bigger::App::updateApp()` __(needs to be overridden)__
        - Update scene objects (`bigger::SceneObject::update()`)
        - Render scene objects (`bigger::SceneObject::draw()`)
      - `imgui` render
      - `bgfx` submit
    - `bigger::App::shutdown()`
      - Release scene objects
      - `bigger::App::releaseSharedResources()` __(needs to be overridden)__
    - `imgui` shutdown
    - `bgfx` shutdown
    - `glfw` shutdown

## App Design

- Always two directional lights
  - Other types of lights or more than two directional lights are not supported
- Intensive use of smart pointers
  - Primitives and materials need to be dynamically instantiated and managed by smart pointers (i.e., either `std::shared_ptr<T>` or `std::unique_ptr<T>`)

## Usage

### Minimal Example

This is a minimal example of using `bigger::App`. This app just shows a blank window and do nothing.

```cpp
#include <bigger/app.hpp>

class MinimalApp final : public bigger::App
{
public:

    MinimalApp() {}

    void initialize(int argc, char** argv) override {}
    void updateApp() override {}
    void releaseSharedResources() override {}
};

int main(int argc, char** argv)
{
    MinimalApp app;
    return app.runApp(argc, argv);
}
```

### Override App Class

The following three methods need to be overridden by the new app class:

- `bigger::App::initialize()`: Initializing the app and instantiating necessary objects living through the app life.
- `bigger::App::updateApp()`: Writing frame-wise update rules and calling `imgui` draw calls.
- `bigger::App::releaseSharedResources()`: Releasing shared resources maintained by the app class (such as vertex buffers).

### (TODO)

### Additional Notes: Screen Capture

`bgfx` provides screen capture functionalities, which of course can be directly called (see the official documentation and examples). For easier use, in `bigger` by default, just inserting the following one-line code into `update()` can capture the screen:

```
bgfx::requestScreenShot(BGFX_INVALID_HANDLE, "/path/to/output");
```

Note: On macOS, using the Metal backend somehow fails to capture the screen (not sure why). A possible quick fix is to use OpenGL.

## License

MIT License

## Contribution

Issue reports & pull requests are highly welcomed.
