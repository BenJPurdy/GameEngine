#version 450 

layout(location = 0) in vec3 position;

uniform mat4 viewProjection;
uniform mat4 transform;

void main()
{
    gl_Position = viewProjection * transform * vec4(position, 1.0);
}