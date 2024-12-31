#version 400

in vec3 color;
in vec3 normal;
out vec3 outputColor;

uniform vec3 lightDir1;
uniform vec3 lightDir2;

void main()
{
	
	outputColor = color + color * max(dot(normal, lightDir1), 0.0) + color * max(dot(normal, lightDir2), 0.0);
}