#version 450

layout (location = 0) out vec4 fragColour;


struct VertexOut
{
    vec4 color;
    vec2 uv;
    vec2 tiling;
};

layout (location = 0) in VertexOut inData;

void main()
{
    vec4 texCol = inData.color;

    fragColour = texCol;
}