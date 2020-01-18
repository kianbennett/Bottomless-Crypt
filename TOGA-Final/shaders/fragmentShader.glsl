#version 330 core
out vec4 FragColor;

uniform vec4 colour; // uniforms are global and can be accessed from the CPU (i.e. the c++ program)

void main()
{
    FragColor = vec4(colour);
} 