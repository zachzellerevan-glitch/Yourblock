#version 330 core
out vec4 FragColor;

//in vec3 vColor;
in vec2 vTexture;
flat in int Layer;
uniform sampler2DArray u_TextureArray;

void main() {
    vec2 FlippedUV = vec2(vTexture.x,1.0 - vTexture.y);
    FragColor = texture(u_TextureArray,vec3(FlippedUV,float(Layer)));
}