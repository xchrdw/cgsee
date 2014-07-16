#version 150

layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform vec2 imageSize;
uniform vec2 viewport;
uniform vec2 aspect;
uniform vec2 pan;
uniform vec2 pixels;

in int v_instance[1];

out float v_linePos;
out float v_scale;

void main(void) {
	if(v_instance[0] <= pixels.x) { // vertical lines
		float xpos = 1 - fract(imageSize.x * pan.x / aspect.x);
		v_scale = viewport.y;
		vec4 pos = vec4((v_instance[0] + xpos)/pixels.x *2-1,-1,0,1);
		gl_Position = pos;
		v_linePos = 0.0;
		EmitVertex();
		gl_Position = pos + vec4(0,2,0,0);
		v_linePos = 1.0;
		EmitVertex();
	} else { // horizontal lines
		float ypos = 1 - fract(imageSize.y * pan.y / aspect.y);
		v_scale = viewport.x;
		vec4 pos = vec4(-1,(v_instance[0] - floor(pixels.x) - 1 + ypos)/pixels.y *2-1,0,1);
		gl_Position = pos;
		v_linePos = 0.0;
		EmitVertex();
		gl_Position = pos + vec4(2,0,0,0);
		v_linePos = 1.0;
		EmitVertex();
	}
    EndPrimitive();
}
