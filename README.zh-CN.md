# YourBlock

**[简体中文](README.zh-CN.md) | [English](README.md)**

一个用 **C++20** 和 **OpenGL 3.3** 编写的体素沙盒游戏（Minecraft 风格），构建在一个类 Hazel 的小型引擎之上——Application 单例 + LayerStack + 类型化事件系统。

> 个人学习项目。引擎和游戏同在一个代码库，方便从头到尾通读。

<!-- TODO: 加截图 -->
<!-- ![截图](screenshot.png) -->

## 功能特性

- **无限流式体素世界** —— chunk 在玩家周围的线程池上异步生成，远离后自动卸载。地形当前是平坦的。
- **第一人称玩家控制器**
  - AABB 碰撞，**逐轴解析**（X → Z → Y）——贴墙滑行、拐角处理自然获得
  - 重力、跳跃、冲刺（按住 `Left Ctrl`）
  - **Y 轴子步移动**——任何帧率下都不会隧穿
- **方块交互**
  - 通过体素光线投射挖掘和放置方块
  - 瞄准的方块用 3D 线框高亮
- **2D UI 叠加层系统** —— 一个四边形渲染器（`pos + uv + color`），目前画准星，可扩展画热栏、菜单、文字
- **方块纹理图集**，每个方块独立配置顶面/侧面/底面纹理（`config/BlockTexture.json`）

## 操作

| 输入 | 动作 |
|---|---|
| `W` `A` `S` `D` | 移动 |
| `Space` | 跳跃 |
| `Left Ctrl` | 冲刺 |
| 鼠标 | 视角 |
| 左键 | 挖掘方块 |
| 右键 | 放置方块 |
| `Esc` | 捕获 / 释放光标 |

调试按键（在 `GameLayer` 中）：`V` 在相机处放置方块，`C` 清除，`X` 打印位置/朝向。

## 架构

引擎参考 [Hazel](https://github.com/TheCherno/Hazel)：

- `Application` 单例拥有主循环和 `LayerStack`
- 各层（`GameLayer`、`UILayer` 等）实现 `OnAttach / OnDetach / OnUpdate / OnEvent`
- 类型化事件系统（`EventDispatcher` + 编译期事件类注册）分发输入和窗口事件
- 层可以按游戏状态启用 / 禁用，让未来的主菜单和游戏画面干净地切换

渲染按坐标系划分：

- **`Renderer2D`** —— 屏幕空间四边形（`pos + uv + color`），画 UI
- **`WireframeRenderer`** —— 世界空间线段，画方块高亮
- `Shader`、`TextureArray`、`Vertex` —— 方块网格与纹理

## 项目结构

```
src/
  Core/     引擎：App, Window, LayerStack, Event, Input, Camera, Timer, ThreadPool
  Game/     GameLayer —— 主游戏逻辑
  Player/   玩家物理与 AABB 碰撞
  Render/   Shader, Texture, TextureArray, Renderer2D, WireframeRenderer, Vertex
  World/    Block 注册表, Chunk, ChunkMesher, World, Raycaster
  UI/       UILayer —— 2D 叠加层
assets/
  Shader/   GLSL 着色器
  Texture/  方块纹理
config/
  BlockTexture.json   方块纹理配置
```

## 构建

### 前置依赖

- CMake ≥ 3.20
- 支持 C++20 的编译器
- Git submodule：`libs/glfw`、`libs/Imgui`

### 步骤

```bash
git clone --recursive https://github.com/<你的用户名>/YourBlock.git
cd YourBlock
cmake -S . -B build
cmake --build build
```

如果克隆时没加 `--recursive`，先拉取 submodule：

```bash
git submodule update --init --recursive
```

可执行文件生成在 `build/`。

> **Windows 说明：** 本项目用 MinGW-w64 开发。Windows 上可能需要显式指定生成器，例如 `cmake -S . -B build -G "MinGW Makefiles"`。任何 CMake 支持的生成器应该都能用。

## 依赖

| 库 | 用途 | 引入方式 |
|---|---|---|
| GLFW | 窗口 / 输入 | git submodule |
| GLM | 数学 | vendored 头文件 |
| GLAD | OpenGL 3.3 加载器 | vendored |
| stb_image | 图片加载 | vendored |
| Dear ImGui (docking) | 已捆绑，暂未使用 | git submodule |

## 开发注意

- 源文件用 `file(GLOB_RECURSE)` **在 configure 时**收集——新增 `.cpp` 后要重新运行 `cmake -S . -B build`。
- `assets/` **在 configure 时**复制到构建目录——新增着色器/纹理后要重新 configure，否则游戏读到的是旧副本。

## 路线图

- 物品栏 / 方块选择（目前固定放置 SAND）
- 文字渲染（字体图集）——FPS 与坐标调试叠加层
- 主菜单 / 暂停画面（层状态切换）
- 更多方块类型和程序化（高度图）地形
- 世界存档 / 读档
- 音频

## 许可证

本项目使用 [MIT License](LICENSE)。
