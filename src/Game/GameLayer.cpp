#include "GameLayer.h"



namespace Engine{
    void GameLayer::OnAttach(){
        std::cout << "GameLayer::OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        
        TextureArray::Get().LoadTexture("assets/Texture");
        
        BlockRegistry::Get();
        
        //m_Chunk = std::make_unique<Chunk>(m_ChunkCoord);
        //m_Chunk->GenerateFlatChunk();
        //m_Chunk->RebuildMesh();

        m_World = std::make_unique<World>(10);
        m_Player = std::make_unique<Player>(glm::vec3(8.0f,30.0f,8.0f));
        m_Shader = std::make_unique<Shader>("assets/Shader/Vertex.glsl","assets/Shader/Fragment.glsl");
        m_Wireframe = std::make_unique<WireframeRenderer>();
        m_Camera = std::make_unique<Camera>(90.0f,1920.0f/1080.0f,0.1f,500.0f);
        m_Camera->SetPosition(glm::vec3(8.0f,30.0f,8.0f));
        m_Camera->SetRotation(-90.0f,-30.0f);
        glEnable(GL_DEPTH_TEST);
    }
    
    void GameLayer::OnUpdate(float dt){
        if(m_FirstFrame){
            glfwSetInputMode(Application::GetApp().GetWindow().GetWindowHandle(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);
            Input::Get().ResetMouseDelta();
             
            m_FirstFrame = false;
        }

        glClearColor(0.5f,0.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
        m_Shader->Use();

        if(m_GameState->State == GameState::Playing){
            glm::vec3 moveDir(0.0f);
            glm::vec3 frontH = glm::normalize(glm::vec3(m_Camera->GetFront().x, 0.0f, m_Camera->GetFront().z));
            if(Input::IsKeyPressed(GLFW_KEY_W)) moveDir += frontH;
            if(Input::IsKeyPressed(GLFW_KEY_S)) moveDir -= frontH;
            if(Input::IsKeyPressed(GLFW_KEY_D)) moveDir += m_Camera->GetRight();
            if(Input::IsKeyPressed(GLFW_KEY_A)) moveDir -= m_Camera->GetRight();
            bool sprint = Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL);
            if(glm::length(moveDir) > 0.0f) moveDir = glm::normalize(moveDir);
            bool jump = Input::IsKeyPressed(GLFW_KEY_SPACE);
            m_Player->Update(dt, moveDir, jump, sprint, *m_World);
            m_TargetPov = sprint ? 100.0f : 90.0f;
            float cur = m_Camera->GetPov();
            m_Camera->SetPov(cur + (m_TargetPov - cur) * std::min(1.0f, 10.0f * dt));
            

            if(Input::IsKeyPressed(GLFW_KEY_V)){
                //std::cout<<"Pos:"<<m_Camera->GetPosition().x<<","<<m_Camera->GetPosition().y<<","<<m_Camera->GetPosition().z<<std::endl;
                std::cout<<"Pos:"<<m_Camera->GetPosition()<<std::endl;
                m_World->SetBlock(m_Camera->GetPosition().x,m_Camera->GetPosition().y,m_Camera->GetPosition().z,BlockType::SAND);
            }
            if(Input::IsKeyPressed(GLFW_KEY_C)){
                //std::cout<<"Pos:"<<m_Camera->GetPosition().x<<","<<m_Camera->GetPosition().y<<","<<m_Camera->GetPosition().z<<std::endl;
                std::cout<<"Pos:"<<m_Camera->GetPosition()<<std::endl;
                m_World->SetBlock(m_Camera->GetPosition().x,m_Camera->GetPosition().y,m_Camera->GetPosition().z,BlockType::AIR);
            }
            if(Input::IsKeyPressed(GLFW_KEY_X)){
                //std::cout<<"Pos:"<<m_Camera->GetPosition().x<<","<<m_Camera->GetPosition().y<<","<<m_Camera->GetPosition().z<<std::endl;
                std::cout<<"Pos:"<<m_Camera->GetPosition()<<std::endl;
                std::cout<<"Front:"<<m_Camera->GetFront().x<<","<<m_Camera->GetFront().y<<","<<m_Camera->GetFront().z<<std::endl;
            }

            if(m_MouseLeftHold){
                float CurTime = Timer::Get().GetCurrentTime();
                if(CurTime - m_LastBreakTime >= m_BreakInterval){
                    BreakBlock();
                    m_LastBreakTime = CurTime;
                }
            }

            if(m_MouseRightHold){
                float CurTime = Timer::Get().GetCurrentTime();
                if(CurTime - m_LastPlaceTime >= m_PlaceInterval){
                    PlaceBlock();
                    m_LastPlaceTime = CurTime;
                }
            }

            auto [dx,dy] = Input::GetDeltaMousePos();
            m_Camera->CameraView(dx,dy);
        }
        
       
        

        m_Camera->SetPosition(m_Player->GetEyePosition());
        if(m_Player->GetPosition().y < -100.0f){
            m_Player->Teleport(glm::vec3(8.0f,30.0f,8.0f));
        }

        glm::mat4 view = m_Camera->GetViewMartix();
        glm::mat4 projection = m_Camera->GetProjectionMartix();
        glm::mat4 model = glm::mat4(1.0f);
        m_Shader->SetUniform("u_View",view);
        m_Shader->SetUniform("u_Projection",projection);
        m_Shader->SetUniform("u_Model",model);
        
        TextureArray::Get().Bind(0);    
        m_Shader->SetUniform("u_TextureArray",0);

        //m_Chunk->Render(*m_Shader);
        m_World->Update(m_Camera->GetPosition());
        m_World->Render(*m_Shader);
        RaycastHit Ray;
        Ray = Raycaster::TraverseRay(10, m_Camera->GetPosition(), m_Camera->GetFront(), m_World.get());
        if(Ray.Hit)
            m_Wireframe->DrawBlockBox(Ray.HitPos, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), view, projection);

        
    }

    void GameLayer::OnDetach(){
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        m_Wireframe.reset();
        m_Shader.reset();
    }

    void GameLayer::OnEvent(Event & event){
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowFocusEvent>([this](WindowFocusEvent & event){
            if(event.IsFocused()){
                glfwSetInputMode(Application::GetApp().GetWindow().GetWindowHandle(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);
                Input::Get().SetWindowFocus(true);
                Input::Get().ResetMouseDelta();
            }else{
                glfwSetInputMode(Application::GetApp().GetWindow().GetWindowHandle(),GLFW_CURSOR,GLFW_CURSOR_NORMAL);
                Input::Get().SetWindowFocus(false);
            }
            return false; //m_Handle
        });

        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent & event){
            if(event.GetKeyCode() == GLFW_KEY_ESCAPE){
                if(!m_ESC){
                    glfwSetInputMode(Application::GetApp().GetWindow().GetWindowHandle(),GLFW_CURSOR,GLFW_CURSOR_NORMAL);
                    Input::Get().SetWindowFocus(false);
                    m_ESC = true;
                    m_GameState->State = GameState::Paused;
                }else{
                    glfwSetInputMode(Application::GetApp().GetWindow().GetWindowHandle(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);
                    Input::Get().SetWindowFocus(true);
                    Input::Get().ResetMouseDelta();
                    m_ESC = false;
                    m_GameState->State = GameState::Playing;
                }
            }
            if(event.GetKeyCode() == GLFW_KEY_1 && event.GetKeyCode() <= GLFW_KEY_9){
                m_Hotbar.SelectedSlot = 0;
            }
            if(event.GetKeyCode() == GLFW_KEY_2 && event.GetKeyCode() <= GLFW_KEY_9){
                m_Hotbar.SelectedSlot = 1;
            }
            if(event.GetKeyCode() == GLFW_KEY_3 && event.GetKeyCode() <= GLFW_KEY_9){
                m_Hotbar.SelectedSlot = 2;
            }
            if(event.GetKeyCode() == GLFW_KEY_4 && event.GetKeyCode() <= GLFW_KEY_9){
                m_Hotbar.SelectedSlot = 3;
            }
            if(event.GetKeyCode() == GLFW_KEY_5 && event.GetKeyCode() <= GLFW_KEY_9){
                m_Hotbar.SelectedSlot = 4;
            }
            if(event.GetKeyCode() == GLFW_KEY_6 && event.GetKeyCode() <= GLFW_KEY_9){
                m_Hotbar.SelectedSlot = 5;
            }
            if(event.GetKeyCode() == GLFW_KEY_7 && event.GetKeyCode() <= GLFW_KEY_9){
                m_Hotbar.SelectedSlot = 6;
            }
            if(event.GetKeyCode() == GLFW_KEY_8 && event.GetKeyCode() <= GLFW_KEY_9){
                m_Hotbar.SelectedSlot = 7;
            }
            if(event.GetKeyCode() == GLFW_KEY_9 && event.GetKeyCode() <= GLFW_KEY_9){
                m_Hotbar.SelectedSlot = 8;
            }
            return false;
        });

        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent & event){
            auto [width,height] = event.GetWindowSize();
            float aspect = (float)width / (float)height;
            m_Camera->SetAspectRatio(aspect);
            return false;
        });

        dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent & event){
            if(event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT){
                m_MouseLeftHold = true;
                BreakBlock();
                m_LastBreakTime = Timer::Get().GetCurrentTime();
                return false;
            }
            return false;
        });

        dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent & event){
            if(event.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT){
                m_MouseLeftHold = false;
                return false;
            }
            return false;
        });

        dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent & event){
            if(m_GameState->State != GameState::Playing) return false;
            if(event.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT){
                m_MouseRightHold = true;
                PlaceBlock();
                m_LastPlaceTime = Timer::Get().GetCurrentTime();
                return false;
            }
            return false;
        });

        dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent & event){
            if(event.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT){
                m_MouseRightHold = false;
                return false;
            }
            return false;
        });

        dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent & event){
            float y = event.GetYOffset();
            if(y == 0.0f) return false;
            int delta = (y > 0.0f) ? -1 : 1;
            int next = (int(m_Hotbar.SelectedSlot) + delta + 9) % 9;
            m_Hotbar.SelectedSlot = (unsigned int)next;
            return false;
        });
    }

    void GameLayer::BreakBlock(){
        RaycastHit Ray;
        Ray = Raycaster::TraverseRay(10,m_Camera->GetPosition(),m_Camera->GetFront(),m_World.get());
        if(!Ray.Hit) return;
        m_World->SetBlock(Ray.HitPos,BlockType::AIR);
    }
    
    void GameLayer::PlaceBlock(){
        RaycastHit Ray;
        Ray = Raycaster::TraverseRay(10,m_Camera->GetPosition(),m_Camera->GetFront(),m_World.get());
        if(!Ray.Hit) return;
        glm::ivec3 PlacePos = Ray.HitPos + Ray.HitFaceNormal;

        //PlayerAABBcollision
        glm::vec3 min = m_Player->GetMinVertex();
        glm::vec3 max = m_Player->GetMaxVertex();
        if (PlacePos.x < max.x && min.x < PlacePos.x + 1 &&
        PlacePos.y < max.y && min.y < PlacePos.y + 1 &&
        PlacePos.z < max.z && min.z < PlacePos.z + 1)
            return;
        // glm::ivec3 CamPos = glm::ivec3(glm::floor(m_Camera->GetPosition()));
        // if(PlacePos == CamPos) return;
        m_World->SetBlock(PlacePos, GetSelectedBlock());
    }
}