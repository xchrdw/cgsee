#version 150

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

//in vec3 o_normal[3];
//uniform mat4 view;

out vec3 normal;

void main(void) {

	vec4 vector1 = gl_in[1].gl_Position - gl_in[0].gl_Position;
	vec4 vector2 = gl_in[2].gl_Position - gl_in[0].gl_Position;

	vec3 vectorCross = cross(vector2.xyz, vector1.xyz);

	normal=vectorCross.xyz;

	gl_Position=gl_in[0].gl_Position;
	normal=vectorCross.xyz;
	EmitVertex();
	
	gl_Position=gl_in[1].gl_Position;
	normal=vectorCross.xyz;
	EmitVertex();

	gl_Position=gl_in[2].gl_Position;
	normal=vectorCross.xyz;
	EmitVertex();

	EndPrimitive();
}