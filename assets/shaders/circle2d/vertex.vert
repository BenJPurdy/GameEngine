#version 450

layout (location = 0) in vec3 worldPos;
layout (location = 1) in vec3 localPos;
layout (location = 2) in vec4 colour;
layout (location = 3) in float thickness;
layout (location = 4) in float fade;
layout (location = 5) in int eID;

layout (std140, binding = 0) uniform Camera
{
    mat4 viewProjection;
};

struct VertexOut
{
    vec3 pos;
    vec4 col;
    float thickness;
    float fade;
};

layout (location = 0) out VertexOut outData;
layout (location = 4) out flat int vertID;

void main()
{
    outData.pos = localPos;
    outData.col = colour;
    outData.thickness = thickness;
    outData.fade = fade;
    vertID = eID;

    gl_Position = viewProjection * vec4(worldPos, 1.0);
}