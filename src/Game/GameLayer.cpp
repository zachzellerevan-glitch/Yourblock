#include "GameLayer.h"

namespace Engine{
    void GameLayer::OnAttach(){
        std::cout << "GameLayer::OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        float Vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
        };

        glGenVertexArrays(1,&m_VAO);
        glGenBuffers(1,&m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(Vertices),Vertices,GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void *)0);
        glEnableVertexAttribArray(0);

        m_Shader = std::make_unique<Shader>("assets/Shader/Vertex.vs","assets/Shader/Fragment.fs");

    }
    
    void GameLayer::OnUpdate(){
        glClearColor(0.5f,0.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
        m_Shader->Use();
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
    }

    void GameLayer::OnDetach(){
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        m_Shader.reset();
    }
}