# YourBlock：玩家 AABB 碰撞实现笔记

> 项目：C++20 + OpenGL 3.3 体素沙盒（Minecraft 风格）
> 涉及文件：
> - `src/Player/Player.*`（核心改动）
> - `src/Game/GameLayer.cpp`（接线）
> - `src/Core/Camera/Camera.h`（加一个 `GetRight()`）
> - `src/World/World.cpp`（修 Y 越界）
> 日期：2026-08-10

---

## 0. 核心思路（一句话）

每帧把玩家速度拆成 X/Y/Z 三个轴，**一次只动一个轴**，动完立刻检测"现在的 AABB 是否与任何实心方块重叠"，重叠就**沿该轴把玩家 clamp 回方块表面并把该轴速度清零**。三个轴分开处理，天然得到"贴墙滑行"，避免斜向卡墙角。

---

## 1. Player 的 AABB 定义

方块是 `1×1×1` 单位立方体，位于整数坐标 `[bx, bx+1)`。玩家盒子以**脚底中心**为锚点：

```
halfWidth = Weight / 2 = 0.6 / 2 = 0.3
min = (px - 0.3,  py,        pz - 0.3)
max = (px + 0.3,  py + 1.8,  pz + 0.3)
```

- `m_Position` 存脚底中心；AABB 顶在 `py`，高 1.8（`Player::Height`）。
- 眼睛在 `py + 1.6`（`EyeHeight`），相机跟随 `position + (0, 1.6, 0)`。
- **X/Z 对称**（±halfWidth），**Y 不对称**（下 0、上 Height）——这是 Y 轴响应特殊处理的原因。

---

## 2. 碰撞检测：候选方块 + 精确相交测试

**① 候选范围**（方块坐标全部 `floor`）：

```
for bx in floor(min.x)..floor(max.x)
  for by in floor(min.y)..floor(max.y)
    for bz in floor(min.z)..floor(max.z)
```

玩家 AABB 最多覆盖 `3×4×3 ≈ 36` 个方块，每帧查几十次 `GetBlock`，**无性能压力**。

**② 精确相交测试**（方块是左闭右开 `[b, b+1)`）：

```
重叠 ⟺  bx < max.x && min.x < bx+1 &&
         by < max.y && min.y < by+1 &&
         bz < max.z && min.z < bz+1
```

注意是 `<` 不是 `<=`，这一步过滤掉"刚好贴着"的邻居（如站地上时脚下的方块）。

**③ 实心判断**：
```cpp
BlockRegistry::Get().GetAttribution(world.GetBlock(bx, by, bz)).IsSolid
```

> 浮点细节：当 `max` 恰好是整数（落地后 `max.y == 20.0`），`floor(max)` 会多扫一个方块，靠②的精确测试兜掉。也可用 `floor(max - 1e-6)` 避开。知道有"左闭右开 + 整数边界"这个坑即可。

---

## 3. 碰撞响应：逐轴 clamp 到方块表面

单轴移动后找出所有重叠实心块，取**沿移动方向最近的面**一次 clamp 解决全部重叠（因为只动了一轴，所有重叠块都在移动方向前方）。

**X 轴**（Z 轴完全对称）：
- `vel.x > 0`：`px = min(hits.bx) - 0.3`（撞到 -X 面）
- `vel.x < 0`：`px = max(hits.bx + 1) + 0.3`（撞到 +X 面）
- 然后 `vel.x = 0`

**Y 轴**（不对称）：
- 下落 `vel.y < 0`：`py = max(hits.by + 1)`（脚底对齐方块顶面），`vel.y = 0`，**`OnGround = true`**
- 上升 `vel.y > 0`：`py = min(hits.by) - 1.8`（头顶对齐方块底面），`vel.y = 0`

落地时**把 `py` 精确设为方块顶面坐标**，不是退回原位置——否则每帧重力拖下去一点又弹回来，会抖动。

---

## 4. 为什么必须逐轴

- 不逐轴 → 斜向撞墙角时 X/Z 同时穿透，不知道该往哪弹 → 卡墙角、贴墙走不动。
- 逐轴 → 免费得到：贴墙滑行、绕墙角、水平移动不受垂直影响。
- 顺序：**X → Z → Y**（先水平后垂直，落地判定最后做）。

---

## 5. 重力、跳跃、OnGround

每帧：
```
vel.y -= GRAVITY * dt
vel.y = max(vel.y, TerminalVelocity)
if (jump && OnGround) { vel.y = JUMP_VELOCITY; OnGround = false; }
```

初始常量（单位：格/秒）：
- `GRAVITY ≈ 25`（想要更重可试 30~32）
- `JUMP_VELOCITY = sqrt(2·G·h)`，跳 1.25 格（能上 1 格台阶）≈ 7.9，取 `8.0`
- `TerminalVelocity ≈ -50`

`OnGround` **只在"下落且命中"时置 true**，不要用"脚下有无方块"判断。

---

## 6. 防隧穿（高速穿墙）

离散逐轴有个前提：**单帧位移 < 1 格**。10 格/秒 @ 60fps = 0.17 格/帧，安全；掉到 5fps = 2 格/帧会穿墙。

- **简单法**：每帧限 `move = min(move, 0.9f)`，不够就拆子步循环。
- **彻底法**：扫掠碰撞（沿轴类 DDA 求首次接触时刻）——思路可参考自己的 `Raycaster.cpp`（Amanatides & Woo 遍历）。
- 工程上先做离散版 + 限速即可。

---

## 7. 集成进本项目的注意点（动手前必读）

1. **`World::GetBlock` 的 Y 越界是 UB**：`Chunk::GetIndex` 对 y<0 会算出负下标，索引 `std::array` 越界。加重力前**必改**：`y < 0 || y >= Chunk::HEIGHT` 一律返回 AIR / return。另外 `World.cpp:81` 的 `> Chunk::HEIGHT` 应为 `>=`（y=256 现在会越界）。
2. **未加载 chunk 返回 AIR**：碰撞只在玩家附近查询，加载半径内安全；但要保证玩家不会掉进边界外的虚空。
3. **世界坐标 → chunk 转换直接用现成的 `World::GetBlock`**，碰撞代码不要自己再算 chunk。

---

## 8. 具体实现：Player 类

### 8.1 头文件（`src/Player/Player.h`）

```cpp
namespace Engine {
    class World; // 前置声明，避免循环 include

    class Player {
    public:
        // —— 尺寸常量 ——
        static constexpr float Weight = 0.6f;            // 宽(格)
        static constexpr float Height = 1.8f;            // 高(格)
        static constexpr float EyeHeight = 1.6f;         // 眼高(格)
        static constexpr float HalfWidth = Weight / 2.0f;// 0.3

        // —— 物理常量 ——
        static constexpr float WalkSpeed    = 4.3f;      // 格/秒
        static constexpr float JumpVelocity = 8.0f;      // 格/秒
        static constexpr float Gravity      = 25.0f;     // 格/秒²
        static constexpr float TerminalVelo = -50.0f;    // 终端速度

        Player(const glm::vec3 & spawnPos);

        // —— 几何访问器 ——
        glm::vec3 GetMinVertex() const;   // AABB 下角
        glm::vec3 GetMaxVertex() const;   // AABB 上角
        glm::vec3 GetEyePosition() const; // 相机锚点 = position + (0, EyeHeight, 0)

        const glm::vec3 & GetPosition() const { return m_Position; }
        bool IsOnGround() const { return m_OnGround; }

        // —— 每帧更新：GameLayer 调一次，输入抽象成参数 ——
        void Update(float dt, const glm::vec3 & moveDir, bool jump, World & world);

    private:
        // 三个单轴移动+碰撞，每个只动一轴
        void MoveAndCollideX(float dt, World & world);
        void MoveAndCollideZ(float dt, World & world);
        void MoveAndCollideY(float dt, World & world);
        // 返回 AABB 在 pos 处覆盖的所有实心方块坐标
        std::vector<glm::ivec3> QuerySolidBlocks(const glm::vec3 & pos, World & world) const;

        glm::vec3 m_Position;   // 脚底中心
        glm::vec3 m_Velocity;
        bool m_OnGround = false;
    };
}
```

说明：
- `Update` 用 `World&` 参数传进来，不在 Player 里存指针——避免生命周期耦合。
- `m_Position` 沿用脚底锚点。`OnGround` 加 `m_` 前缀并初始化为 false，与全项目命名一致。
- `HalfWidth = Weight / 2` 是常量表达式（C++17 起 `static constexpr` 隐式 inline，无需在 cpp 再定义）。

### 8.2 访问器

```cpp
glm::vec3 Player::GetMinVertex() const {
    return m_Position - glm::vec3(HalfWidth, 0.0f, HalfWidth);
}
glm::vec3 Player::GetMaxVertex() const {
    return m_Position + glm::vec3(HalfWidth, Height, HalfWidth);
}
glm::vec3 Player::GetEyePosition() const {
    return m_Position + glm::vec3(0.0f, EyeHeight, 0.0f);
}
```

### 8.3 Update

```cpp
void Player::Update(float dt, const glm::vec3 & moveDir, bool jump, World & world) {
    // 1. 水平速度直接由输入决定（简单版；想要惯性以后再加速/摩擦）
    m_Velocity.x = moveDir.x * WalkSpeed;
    m_Velocity.z = moveDir.z * WalkSpeed;

    // 2. 跳跃：只在地面才能起跳
    if (jump && m_OnGround) {
        m_Velocity.y = JumpVelocity;
        m_OnGround = false;
    }

    // 3. 重力 + 终端速度
    m_Velocity.y -= Gravity * dt;
    if (m_Velocity.y < TerminalVelo) m_Velocity.y = TerminalVelo;

    // 4. 逐轴碰撞：先水平后垂直（顺序 X → Z → Y）
    MoveAndCollideX(dt, world);
    MoveAndCollideZ(dt, world);
    MoveAndCollideY(dt, world);
}
```

### 8.4 单轴碰撞

X 轴（Z 轴换轴即可）：

```cpp
void Player::MoveAndCollideX(float dt, World & world) {
    glm::vec3 target = m_Position;
    target.x += m_Velocity.x * dt;

    auto hits = QuerySolidBlocks(target, world);
    if (hits.empty()) { m_Position.x = target.x; return; }

    if (m_Velocity.x > 0) {                        // 向右撞到方块 -X 面
        float nearest = std::numeric_limits<float>::max();
        for (auto & b : hits) nearest = std::min(nearest, (float)b.x);
        m_Position.x = nearest - HalfWidth;
    } else {                                       // 向左撞到 +X 面
        float farthest = std::numeric_limits<float>::lowest();
        for (auto & b : hits) farthest = std::max(farthest, (float)b.x + 1.0f);
        m_Position.x = farthest + HalfWidth;
    }
    m_Velocity.x = 0;
}
```

Y 轴（不对称：脚底 0、头顶 Height；落地置 OnGround）：

```cpp
void Player::MoveAndCollideY(float dt, World & world) {
    glm::vec3 target = m_Position;
    target.y += m_Velocity.y * dt;

    auto hits = QuerySolidBlocks(target, world);
    if (hits.empty()) { m_Position.y = target.y; return; }

    if (m_Velocity.y < 0) {                        // 下落 → 落地
        float top = std::numeric_limits<float>::lowest();
        for (auto & b : hits) top = std::max(top, (float)b.y + 1.0f);
        m_Position.y = top;                        // 脚底精确贴方块顶面
        m_OnGround = true;
    } else {                                       // 上升 → 撞头
        float bottom = std::numeric_limits<float>::max();
        for (auto & b : hits) bottom = std::min(bottom, (float)b.y);
        m_Position.y = bottom - Height;
    }
    m_Velocity.y = 0;
}
```

### 8.5 查询函数

```cpp
std::vector<glm::ivec3> Player::QuerySolidBlocks(const glm::vec3 & pos, World & world) const {
    glm::vec3 min = pos - glm::vec3(HalfWidth, 0.0f, HalfWidth);
    glm::vec3 max = pos + glm::vec3(HalfWidth, Height, HalfWidth);

    std::vector<glm::ivec3> hits;
    for (int bx = (int)std::floor(min.x); bx <= (int)std::floor(max.x); bx++)
    for (int by = (int)std::floor(min.y); by <= (int)std::floor(max.y); by++)
    for (int bz = (int)std::floor(min.z); bz <= (int)std::floor(max.z); bz++) {
        // 左闭右开的精确相交测试（过滤掉刚好贴着的方块）
        if (!(bx < max.x && min.x < bx + 1 &&
              by < max.y && min.y < by + 1 &&
              bz < max.z && min.z < bz + 1))
            continue;

        BlockType type = world.GetBlock(bx, by, bz);
        if (BlockRegistry::Get().GetAttribution(type).IsSolid)
            hits.push_back({ bx, by, bz });
    }
    return hits;
}
```

---

## 9. GameLayer 接线

```cpp
// OnAttach:
m_Player = std::make_unique<Player>(glm::vec3(8.0f, 30.0f, 8.0f)); // 与相机出生点一致

// OnUpdate 里，替换掉现在直接 m_Camera->CameraMove 的那些行：
glm::vec3 moveDir(0.0f);
glm::vec3 frontH = glm::normalize(glm::vec3(m_Camera->GetFront().x, 0.0f, m_Camera->GetFront().z));
if (IsKeyPressed(W)) moveDir += frontH;
if (IsKeyPressed(S)) moveDir -= frontH;
if (IsKeyPressed(D)) moveDir += m_Camera->GetRight();   // 需要给 Camera 加 GetRight()
if (IsKeyPressed(A)) moveDir -= m_Camera->GetRight();
if (glm::length(moveDir) > 0.0f) moveDir = glm::normalize(moveDir);
bool jump = Input::IsKeyPressed(GLFW_KEY_SPACE);

m_Player->Update(dt, moveDir, jump, *m_World);
m_Camera->SetPosition(m_Player->GetEyePosition());
```

Camera 退化为纯视角对象（`CameraView` 鼠标转向 + `SetPosition` 跟随），`CameraMove` / `EnableFly` 可不再用。

---

## 10. 动手前先做的三件小事

1. **修 `World::GetBlock/SetBlock` 的 Y 越界**（`World.cpp:81`）：`y < 0 || y >= Chunk::HEIGHT` 一律返回 AIR / return。这是**加重力前的必改项**（否则 y<0 时 `GetIndex` 负下标 UB）。
2. **删掉 `World.h:41` 悬空的 `int Gravity;`** —— 重力常量已经进 Player。
3. **给 Camera 加 `GetRight()`**（返回 `m_Right`，天然水平），GameLayer 算水平移动方向要用。

---

## 11. 进阶可选（以后再做）

- **跨台阶**：水平被挡且 `OnGround` 时，尝试向上抬 0.5 格再横移——省得每次上楼梯都跳。
- **跳跃手感**：coyote time（离开平台 0.1s 内仍可跳）+ 跳跃预输入（落地前按下也算）。
- **潜行**：把 Height 临时压到 1.5。
- **扫掠碰撞**：彻底防隧穿，参考自己的 `Raycaster` 思路。
- **下一个 TODO**：`UILayer`（接入 ImGui → `PushOverlay` → 画准星 / FPS 面板 / 热栏选方块）。
