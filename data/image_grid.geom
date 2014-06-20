#version 150

layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform float lines;

in int v_instance[1];

void main(void) {
	if(lines > 0) { // vertical lines
		vec4 pos = vec4(v_instance[0]*(2/lines)-1,-1,0,1);
		gl_Position = pos;
		EmitVertex();
		gl_Position = pos + vec4(0,2,0,0);
		EmitVertex();
	} else { // horizontal lines
		vec4 pos = vec4(-1,v_instance[0]*(-2/lines)-1,0,1);
		gl_Position = pos;
		EmitVertex();
		gl_Position = pos + vec4(2,0,0,0);
		EmitVertex();
	}
    EndPrimitive();
}
