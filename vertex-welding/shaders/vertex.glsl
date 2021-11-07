#version 450 core

layout (location=0) in vec3 vVertex;

uniform mat4 viewProj;

void main(void)
{
	gl_Position = vec4(vVertex,1) * viewProj;
}