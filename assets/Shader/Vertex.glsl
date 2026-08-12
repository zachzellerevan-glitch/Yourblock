#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexture;
layout (location = 2) in int aLayer;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

//out vec3 vColor;
out vec2 vTexture;
flat out int Layer;
void main() {
    gl_Position =  u_Projection * u_View * u_Model * vec4(aPos, 1.0);
    //vColor = aColor;
    vTexture = aTexture;
    Layer = aLayer;
}