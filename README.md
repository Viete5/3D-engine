# Custom OpenGL Engine

Учебный 3D-движок на C++17 и OpenGL. 

## Текущее Состояние

На данный момент реализовано:

- Окно и OpenGL-контекст через GLFW/GLAD.
- Главный цикл приложения: input, time update, scene update, render, swap buffers.
- Обработка клавиатуры и мыши.
- Свободная камера с управлением от первого лица.
- Переключение fullscreen/windowed по `F11`.
- Захват и скрытие курсора для свободного вращения камеры.
- Автоматическое обновление viewport и aspect ratio при изменении размера окна.
- Простая система логирования: `info`, `warning`, `error`, `critical`.
- Система путей `Paths` для `assets/shaders`, `assets/textures`, `assets/models`, `assets/scenes`.
- Математическое ядро: `Vector`, `Vector4`, `Matrix4`.
- Сцена: `Scene`, `SceneObject`, `Transform`, `Camera`, `Light`.
- Рендеринг `Mesh + Material + Shader`.
- Lambert diffuse lighting с поддержкой нескольких источников света в шейдере.
- OpenGL wrappers для VAO, VBO, EBO, Texture, ShaderProgram.
- `TextureLoader` на базе `stb_image`.
- `Model` как контейнер ресурсов модели.
- `ModelLoader` на базе Assimp для загрузки `.obj`, `.gltf`, `.glb` и других форматов Assimp.
- Импорт mesh, material, texture, node transform и object name из модели.
- Поддержка внешних texture-файлов и embedded PNG/JPEG texture из `.glb`.
- Логи статистики загрузки модели.
- CMake-сборка `engine` как static library и `apps/sandbox` как executable.
- Автоматическое копирование runtime DLL рядом с `sandbox.exe` для MinGW/Windows.

## Основные Модули

### Core

`Application` управляет жизненным циклом программы:

- инициализирует GLFW;
- создает окно;
- загружает GLAD;
- подключает input;
- создает сцену через `SceneFactory`;
- запускает главный цикл;
- обновляет `Time`;
- передает input и delta time в сцену;
- вызывает `Renderer`;
- корректно завершает работу.

`Logger` отвечает за вывод сообщений. Ошибки уровня `error` и `critical` пишутся в `std::cerr`, остальные сообщения в `std::cout`.

`Paths` хранит путь к папке `assets` и строит пути к ресурсам

`Time` отвечает за обработку времени


### Platform

`Window` является оберткой над `GLFWwindow`.

Умеет:

- создавать и уничтожать окно;
- проверять закрытие окна;
- делать `swap_buffers`;
- делать `poll_events`;
- переключать fullscreen/windowed;
- захватывать курсор;
- обновлять размер framebuffer и viewport.

`Input` хранит состояние клавиш и mouse delta. GLFW cursor callback привязан через `glfwSetWindowUserPointer`.

### Scene

`Scene` хранит:

```cpp
std::vector<SceneObject> objects;
Camera camera;
std::vector<Light> lights;
```

`SceneObject` представляет объект в сцене:

```cpp
std::string name;
Transform transform;
Material* material;
Mesh* mesh;
```

`Transform` хранит position, rotation, scale и строит model matrix.

`Camera` строит view/projection matrix и обрабатывает движение.

`Light` пока представляет простой источник света с position/color.

### Render

`Mesh` хранит vertex/index data и OpenGL buffers.

`Vertex` сейчас содержит:

```cpp
Vector position;
Vector normal;
float tex_u;
float tex_v;
```

`Material` хранит:

```cpp
Shader* shader;
Vector base_color;
Texture* base_color_texture;
```

`Renderer` проходит по объектам сцены, активирует material/shader, передает uniforms и вызывает `mesh.draw()`.

`Model` является владельцем ресурсов импортированной модели:

```cpp
std::vector<std::unique_ptr<Mesh>> meshes;
std::vector<std::unique_ptr<Material>> materials;
std::vector<std::unique_ptr<Texture>> textures;
std::vector<SceneObject> objects;
```

Это нужно, потому что `SceneObject` хранит только указатели на `Mesh` и `Material`, а кто-то должен владеть реальными ресурсами.

### Assets

`TextureLoader` загружает изображения через `stb_image`.

Поддерживает:

- загрузку из файла;
- загрузку из памяти для embedded textures внутри `.glb`.

`ModelLoader` использует Assimp и конвертирует:

```text
aiScene    -> Model
aiMesh     -> Mesh
aiMaterial -> Material
aiTexture  -> Texture
aiNode     -> SceneObject + Transform + name
```

При загрузке выводится статистика:

```text
Loaded model: ... | meshes=... materials=... textures=... objects=... nodes=...
Model textures: external=... embedded=... reused=...
```

## Sandbox

Текущее sandbox-приложение находится в:

```text
apps/sandbox
```

Оно делает минимальную демонстрацию:

- создает `Application`;
- задает `assets_dir`;
- создает `Sandbox` через `SceneFactory`;
- загружает shader из `assets/shaders`;
- загружает модель тигра из `assets/models/tiger.obj`;
- добавляет объекты модели в сцену;
- позволяет управлять камерой.

Управление:

```text
W/A/S/D      движение камеры
Space        вверх
Left Ctrl    вниз
Mouse        вращение камеры
Q/E          изменить FOV
F11          fullscreen/windowed
Escape       закрыть приложение
```

## Blender / Model Pipeline

Рекомендуемый workflow:

```text
Blender или скачанная модель
        |
        v
.obj + .mtl + textures
или .gltf/.glb
        |
        v
assets/models
assets/textures
        |
        v
ModelLoader
        |
        v
Model -> SceneObject -> Renderer
```

Сейчас движок реально использует diffuse/base color texture. Normal map и roughness texture могут лежать рядом, но пока не участвуют в шейдере.

## Сборка

Требования:

- CMake 3.10+
- C++17 compiler
- GLFW
- GLAD object file в `vendor/glad.o`
- Assimp submodule в `third_party/assimp`
- Windows/MinGW сейчас является основной проверенной конфигурацией

Конфигурация:

```powershell
cmake -S . -B build
```

Сборка sandbox:

```powershell
cmake --build build --target sandbox --config Debug
```

Запуск:

```powershell
.\build\apps\sandbox\sandbox.exe
```

Для Windows/MinGW `apps/sandbox/CMakeLists.txt` копирует рядом с `sandbox.exe` нужные runtime DLL:

```text
libassimp-6d.dll
glfw3.dll
libstdc++-6.dll
libgcc_s_seh-1.dll
libwinpthread-1.dll
```