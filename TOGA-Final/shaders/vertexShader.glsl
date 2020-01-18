#version 330 core
layout (location = 0) in vec3 pos; // the position variable has attribute position 0

uniform mat4 model, projection;

void main()
{
    // No need for view matrix as we aren't using perspective projection
    gl_Position = projection * model * vec4(pos, 1.0f);
}