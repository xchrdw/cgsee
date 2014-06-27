#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float width;
uniform float height;
uniform vec2 aspect;
uniform vec2 pan;
uniform vec2 lines;

in int v_instance[1];

out vec2 v_uv;

void main(void) {
	float xpos = -fract(width * pan.x / aspect.x);
	float ypos = -fract(height * pan.y / aspect.y);
	
	float w = 2/lines.x;
	float h = 2/lines.y;
	
	vec4 pos = vec4(
		(mod(v_instance[0], (ceil(lines.x)+1)) + xpos)/lines.x *2-1 - w/2,
		(floor(v_instance[0] / (ceil(lines.x)+1)) + ypos)/lines.y *2-1 - h/2,
		0,1);

	gl_Position = pos + vec4(w/2,-h/2,0,0);
	v_uv = vec2(1,0);
	EmitVertex();
	gl_Position = pos + vec4(w/2,h/2,0,0);
	v_uv = vec2(1,1);
	EmitVertex();
	gl_Position = pos + vec4(-w/2,-h/2,0,0);
	v_uv = vec2(0,0);
	EmitVertex();
	gl_Position = pos + vec4(-w/2,h/2,0,0);
	v_uv = vec2(0,1);
	EmitVertex();
    EndPrimitive();
}
