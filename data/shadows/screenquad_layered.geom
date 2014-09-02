#version 150 core

layout( triangles ) in;
layout( triangle_strip, max_vertices = 48 ) out;
out int gl_Layer;

in vec2 v_uv[];

out vec3 g_uv;

void main()
{
	for(int layer = 0; layer < 2; ++layer)
	{
		gl_Layer = layer;
		for(int i = 0; i < 3; ++i)
		{
			g_uv = vec3(v_uv[i], layer);
			gl_Position = gl_in[i].gl_Position; 
			EmitVertex();
		}
		EndPrimitive();
	}
}