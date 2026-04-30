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

        m_Shader = std::make_unique<Shader>("assets/Shader/Vertex.vs","assets/Shader/Fragment.fs");

        m_Camera = std::make_unique<Camera>(90.0f,1920/1080,0.1f,500.0f);
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
        
        if(Input::IsKeyPressed(GLFW_KEY_W)){
            //printf("GameLayer::W pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::FORWARD,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_S)){
            //printf("GameLayer::S pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::BACKWARD,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_A)){
            //printf("GameLayer::A pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::LEFT,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_D)){
            //printf("GameLayer::D pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::RIGHT,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_SPACE)){
            //printf("GameLayer::SPACE pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::UP,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)){
            //printf("GameLayer::Left Shift pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::DOWN,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)){
            m_Camera->SetCameraSpeed(10.0f);
        }else{
            m_Camera->SetCameraSpeed(5.0f);
        }
        if(Input::IsKeyPressed(GLFW_KEY_V)){
            std::cout<<"Pos:"<<m_Camera->GetPosition().x<<","<<m_Camera->GetPosition().y<<","<<m_Camera->GetPosition().z<<std::endl;
            m_World->SetBlock(m_Camera->GetPosition().x,m_Camera->GetPosition().y,m_Camera->GetPosition().z,BlockType::SAND);
        }
        if(Input::IsKeyPressed(GLFW_KEY_C)){
            std::cout<<"Pos:"<<m_Camera->GetPosition().x<<","<<m_Camera->GetPosition().y<<","<<m_Camera->GetPosition().z<<std::endl;
            m_World->SetBlock(m_Camera->GetPosition().x,m_Camera->GetPosition().y,m_Camera->GetPosition().z,BlockType::AIR);
        }
        //printf("GameLayer::MousePos::%d,%d\n",(int)Input::GetMouseX(),(int)Input::GetMouseY());
        auto [dx,dy] = Input::GetDeltaMousePos();
        m_Camera->CameraView(dx,dy);

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
    }

    void GameLayer::OnDetach(){
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
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
                }else{
                    glfwSetInputMode(Application::GetApp().GetWindow().GetWindowHandle(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);
                    Input::Get().SetWindowFocus(true);
                    Input::Get().ResetMouseDelta();
                    m_ESC = false;
                }
            }
            return false;
        });

        dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent & event){
            auto [width,height] = event.GetWindowSize();
            float aspect = (float)width / (float)height;
            m_Camera->SetAspectRatio(aspect);
            return false;
        });
    }
}