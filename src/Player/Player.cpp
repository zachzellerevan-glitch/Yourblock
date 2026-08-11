#include "Player.h"
#include "Core/Camera/Camera.h"

namespace Engine{
    Player::Player(const glm::vec3 SpawnPos)
        : m_Position(SpawnPos), m_Velocity(0.0f), m_OnGround(false) {}

    void Player::Teleport(const glm::vec3 & pos){
        m_Position = pos;
    }

    void Player::Update(float dt, const glm::vec3 & moveDir, bool jump, bool sprint, World & world){
        float speed = sprint ? RunSpeed : WalkSpeed;
        m_Velocity.x = moveDir.x * speed;
        m_Velocity.z = moveDir.z * speed;
        // BlockType bt = world.GetBlock(m_Position.x, m_Position.y - 1.0f, m_Position.z);
        // if(bt == BlockType::AIR)
        //     m_OnGround = false;
        // else
        //     m_OnGround = true;
        if(jump && m_OnGround){
            m_Velocity.y = JumpVelocity;
            m_OnGround = false;
        }
        m_Velocity.y -= Gravity * dt;
        if(m_Velocity.y < MaxFallVelocity){
            m_Velocity.y = MaxFallVelocity;
        }
        MoveAndCollideX(dt, world);
        MoveAndCollideZ(dt, world);
        MoveAndCollideY(dt, world);
    }

    std::vector<glm::ivec3> Player::QuerySolidBlock(const glm::vec3 & pos, World & world) const{
        glm::vec3 min = pos - glm::vec3(HalfWidth, 0.0f, HalfWidth);
        glm::vec3 max = pos + glm::vec3(HalfWidth, Height, HalfWidth);

        std::vector<glm::ivec3> hits;
        for(int bx = (int)std::floor(min.x); bx <= (int)std::floor(max.x); bx++)
            for(int by = (int)std::floor(min.y); by <= (int)std::floor(max.y); by++)
                for(int bz = (int)std::floor(min.z); bz <= (int)std::floor(max.z); bz++){
                     // 左闭右开的精确相交测试（过滤掉刚好贴着的方块）
                    if (!(bx < max.x && min.x < bx + 1 &&
                        by < max.y && min.y < by + 1 &&
                        bz < max.z && min.z < bz + 1))
                        continue;
                    BlockType bt = world.GetBlock(bx,by,bz);
                    if(BlockRegistry::Get().GetAttribution(bt).IsSolid)
                        hits.push_back({bx,by,bz});
                }
        return hits;
    }
    
    void Player::MoveAndCollideX(float dt, World & world){
        glm::vec3 target = m_Position;
        target.x += m_Velocity.x * dt;

        auto hits = QuerySolidBlock(target, world);
        if(hits.empty()){ 
            m_Position.x = target.x; 
            return;
        }

        if(m_Velocity.x > 0){// 向右撞到方块 -X 面
            float nearest = std::numeric_limits<float>::max();
            for(auto & b : hits)
                nearest = std::min(nearest, (float)b.x);
            m_Position.x = nearest - HalfWidth;
        }else{// 向左撞到 +X 面
            float farthest = std::numeric_limits<float>::lowest();
            for(auto & b : hits)
                farthest = std::max(farthest, (float)b.x + 1.0f);
            m_Position.x = farthest + HalfWidth;
        }
        m_Velocity.x = 0;
    }

    void Player::MoveAndCollideZ(float dt, World & world){
        glm::vec3 target = m_Position;
        target.z += m_Velocity.z * dt;

        auto hits = QuerySolidBlock(target, world);
        if(hits.empty()){ 
            m_Position.z = target.z; 
            return;
        }

        if(m_Velocity.z > 0){// 向右撞到方块 -Z 面
            float nearest = std::numeric_limits<float>::max();
            for(auto & b : hits)
                nearest = std::min(nearest, (float)b.z);
            m_Position.z = nearest - HalfWidth;
        }else{// 向左撞到 +Z 面
            float farthest = std::numeric_limits<float>::lowest();
            for(auto & b : hits)
                farthest = std::max(farthest, (float)b.z + 1.0f);
            m_Position.z = farthest + HalfWidth;
        }
        m_Velocity.z = 0;
    }

    void Player::MoveAndCollideY(float dt, World & world){
        float remaining = m_Velocity.y * dt;          // 本帧 Y 总位移（可负）
        const float maxStep = 0.5f;                   // 每步最多 0.5 格
        int steps = (int)std::ceil(std::abs(remaining) / maxStep);
        float stepDelta = remaining / steps;          // 每步位移

        for(int i = 0; i < steps; i++){
            glm::vec3 target = m_Position;
            target.y += stepDelta;

            auto hits = QuerySolidBlock(target, world);
            if(hits.empty()){
                m_Position.y = target.y;              // 这步没撞到，走完
                if(m_Velocity.y < 0)
                    m_OnGround = false;
                continue;
            }

            if(m_Velocity.y < 0){                     // 下落 → 落地
                float top = std::numeric_limits<float>::lowest();
                for(auto & b : hits) top = std::max(top, (float)b.y + 1.0f);
                m_Position.y = top;
                m_OnGround = true;
            }else{                                    // 上升 → 撞头
                float bottom = std::numeric_limits<float>::max();
                for(auto & b : hits) bottom = std::min(bottom, (float)b.y);
                m_Position.y = bottom - Height;
            }
            m_Velocity.y = 0;                         // 撞上了，速度清零
            return;                                   // 剩余位移丢弃
        }
    }

}