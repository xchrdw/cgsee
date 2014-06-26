#version 150

layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform float width;
uniform float height;
uniform vec2 aspect;
uniform vec2 pan;
uniform vec2 lines;

in int v_instance[1];

void main(void) {
	if(v_instance[0] <= lines.x) { // vertical lines
		float xpos = 1 - fract(width * pan.x / aspect.x);
		vec4 pos = vec4((v_instance[0] + xpos)/lines.x *2-1,-1,0,1);
		gl_Position = pos;
		EmitVertex();
		gl_Position = pos + vec4(0,2,0,0);
		EmitVertex();
	} else { // horizontal lines
		float ypos = 1 - fract(height * pan.y / aspect.y);
		vec4 pos = vec4(-1,(v_instance[0] - floor(lines.x) - 1 + ypos)/lines.y *2-1,0,1);
		gl_Position = pos;
		EmitVertex();
		gl_Position = pos + vec4(2,0,0,0);
		EmitVertex();
	}
    EndPrimitive();
}
