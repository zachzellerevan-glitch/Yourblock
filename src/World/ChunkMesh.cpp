#include "ChunkMesh.h"

namespace Engine{
    ChunkMesh::~ChunkMesh(){
        if(m_VAO)
            glDeleteVertexArrays(1,&m_VAO);
        if(m_VBO)
            glDeleteBuffers(1,&m_VBO);
        if(m_EBO)
            glDeleteBuffers(1,&m_EBO);
    }
    
    void ChunkMesh::Upload(const std::vector<Vertex> &Verticies, const std::vector<uint32_t> &Indicies){
        if(m_VAO){
            glDeleteVertexArrays(1,&m_VAO);
            glDeleteBuffers(1,&m_VBO);
            glDeleteBuffers(1,&m_EBO);
            m_VAO = m_VBO = m_EBO = 0;
        }

        m_IndexCount = static_cast<GLsizei>(Indicies.size());
        if(Verticies.empty() || Indicies.empty()){
            return;
        }

        glGenVertexArrays(1,&m_VAO);
        glGenBuffers(1,&m_VBO);
        glGenBuffers(1,&m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
        glBufferData(GL_ARRAY_BUFFER,Verticies.size() * sizeof(Vertex),Verticies.data(),GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,Indicies.size() * sizeof(uint32_t),Indicies.data(),GL_STATIC_DRAW);

        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,Position)); //offsetof:获取结构体成员相对于结构体起始地址的字节偏移量
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,TexCoord));
        glEnableVertexAttribArray(1);

        glVertexAttribIPointer(2,1,GL_INT,sizeof(Vertex),(void*)offsetof(Vertex,Layer));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
    
    void ChunkMesh::Draw() const{
        if(m_IndexCount == 0) return;
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES,m_IndexCount,GL_UNSIGNED_INT,nullptr);
    }
}