#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 normal;
in vec3 _normal[];

void main(void) {

	normal=normalize( (_normal[0] + _normal[1] + _normal[2])/3 );

	gl_Position=gl_in[0].gl_Position;
	EmitVertex();
	
	gl_Position=gl_in[1].gl_Position;
	EmitVertex();

	gl_Position=gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}