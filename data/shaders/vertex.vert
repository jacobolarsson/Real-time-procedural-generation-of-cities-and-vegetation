#version 400
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 w2PerspMtx;

out vec3 color;
out vec3 normal;

void main()
{
   gl_Position = w2PerspMtx * vec4(aPosition, 1.0);
   color = vec3(0.4, 0.4, 0.4);
   normal = aNormal;
}