#version 150 core

uniform mat4 lightTransforms[SHADOWMAP_COUNT_1];

layout( triangles ) in;
layout( triangle_strip, max_vertices = SHADOWMAP_COUNT_3 ) out;

out flat int gl_Layer;

void main()
{
	for(int layer = 0; layer < SHADOWMAP_COUNT_1; layer++)
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
