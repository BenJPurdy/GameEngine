#version 450

layout (location = 0) out vec4 fragColour;
layout (location = 1) out int outID;


struct VertexOut
{
    vec4 color;
    vec2 uv;
    vec2 tiling;
};

layout (location = 0) in VertexOut inData;
layout (location = 3) in flat int vertID;

void main()
{
    vec4 texCol = inData.color;
    outID = vertID;
    fragColour = texCol;
}