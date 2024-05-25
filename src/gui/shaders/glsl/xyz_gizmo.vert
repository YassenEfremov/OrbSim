#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 xyz_gizmo_color;

flat out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    //gl_Position = vec4(position, 1.0);
    gl_Position = projection * view * model * vec4(position, 1.0);
    color = xyz_gizmo_color;
}