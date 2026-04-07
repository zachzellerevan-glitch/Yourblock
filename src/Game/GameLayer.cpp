#include "GameLayer.h"



namespace Engine{
    void GameLayer::OnAttach(){
        std::cout << "GameLayer::OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        float Vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
        };

        float vertices[] = {
            // 位置              // 颜色
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // 0 红色
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, // 1 绿色
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // 2 蓝色
            -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // 3 黄色
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // 4 品红
            0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, // 5 青色
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f, // 6 白色
            -0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f  // 7 灰色
        };

        unsigned int indices[] = {
            // 底面
            0, 1, 2,
            2, 3, 0,
            // 顶面
            4, 5, 6,
            6, 7, 4,
            // 前面 (z = 0.5)
            3, 2, 6,
            6, 7, 3,
            // 后面 (z = -0.5)
            0, 1, 5,
            5, 4, 0,
            // 左面 (x = -0.5)
            0, 3, 7,
            7, 4, 0,
            // 右面 (x = 0.5)
            1, 2, 6,
            6, 5, 1
        };

        glGenVertexArrays(1,&m_VAO);
        glGenBuffers(1,&m_VBO);
        glGenBuffers(1,&m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

        //Position attributions
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6* sizeof(float),(void *)0);
        glEnableVertexAttribArray(0);

        //Color attribution
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);

        m_Shader = std::make_unique<Shader>("assets/Shader/Vertex.vs","assets/Shader/Fragment.fs");
        m_Camera = std::make_unique<Camera>(90.0f,1.77f,0.1f,100.0f);
        m_Camera->SetPosition(glm::vec3(0.0f,0.0f,3.0f));
        
        //Problem: If attach gamelayer in app start,program will collaspe
        //glfwSetInputMode(Application::GetApp().GetWindow().GetWindowHandle(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);
        
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
            printf("GameLayer::W pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::FORWARD,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_S)){
            printf("GameLayer::S pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::BACKWARD,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_A)){
            printf("GameLayer::A pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::LEFT,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_D)){
            printf("GameLayer::D pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::RIGHT,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_SPACE)){
            printf("GameLayer::SPACE pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::UP,dt);
        }
        if(Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)){
            printf("GameLayer::Left Shift pressed.\n");
            m_Camera->CameraMove(Camera::MoveDirection::DOWN,dt);
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

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
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
                Input::Get().ResetMouseDelta();
            }else{
                glfwSetInputMode(Application::GetApp().GetWindow().GetWindowHandle(),GLFW_CURSOR,GLFW_CURSOR_NORMAL);
            }
            return false; //m_Handle
        });
    }
}