#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec2 tiling;

layout (std140, binding = 0) uniform Camera
{
    mat4 viewProjection;
};

struct VertexOut
{
    vec4 color;
    vec2 uv;
    vec2 tiling;
};

layout (location = 0) out VertexOut outData;

void main()
{
    outData.color = colour;
    outData.uv = texCoord;
    outData.tiling = tiling;

    gl_Position = viewProjection * vec4(position, 1.0);
}