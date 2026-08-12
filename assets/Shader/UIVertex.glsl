#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;
uniform mat4 u_Transform;
out vec2 v_UV;
out vec4 v_Color;
void main(){
    gl_Position = u_Transform * vec4(aPos, 0.0, 1.0);
    v_UV = aUV;
    v_Color = aColor;
}