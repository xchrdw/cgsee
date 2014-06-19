#version 150 core

in vec3 a_vertex;

out int v_instance;

void main(void)
{
	v_instance = gl_InstanceID;
	gl_Position = vec4(a_vertex, 1.0);
}
