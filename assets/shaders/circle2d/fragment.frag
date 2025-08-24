#version 450

layout (location = 0) out vec4 fragColour;
layout (location = 1) out int outID;


struct VertexOut
{
    vec3 pos;
    vec4 col;
    float thickness;
    float fade;
};

layout (location = 0) in VertexOut inData;
layout (location = 4) in flat int vertID;

void main()
{
    float dist = 1.0 - length(inData.pos);
    vec2 uv = ((inData.pos.xy) + vec2(1.0f))/vec2(2.0f);
    float circle = smoothstep(0.0, inData.fade, dist);
    circle *= smoothstep(inData.thickness + inData.fade, inData.thickness, dist);

    if (circle == 0.0) discard;
    vec4 col = vec4(uv, 0.0f, 1.0f);
    fragColour = col;
    fragColour.a *= circle;
    outID = vertID;
}