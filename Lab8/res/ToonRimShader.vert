#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

out vec3 Normal;

uniform mat4 transform;

void main()
{
	Normal = VertexNormal;
	gl_Position = transform * vec4(VertexPosition, 1.0);
}