#version 150 core

uniform mat4 lightTransforms[4];

layout( triangles ) in;
layout( triangle_strip, max_vertices = 12 ) out;

out flat int gl_Layer;

void main()
{
	for(int layer = 0; layer < 4; layer++)
	{
		gl_Layer = layer;
		for(int i = 0; i < 3; i++)
		{
			gl_Position = lightTransforms[layer] * gl_in[i].gl_Position; 
			EmitVertex();
		}
		EndPrimitive();
	}
}
