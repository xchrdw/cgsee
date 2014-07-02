#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform vec2 imageSize;
uniform vec2 aspect;
uniform vec2 pan;
uniform vec2 pixels;

in int v_instance[1];

out vec2 v_uv;

void main(void) {
	float xpos = -fract(imageSize.x * pan.x / aspect.x);
	float ypos = -fract(imageSize.y * pan.y / aspect.y);
	
	float w = 2/pixels.x;
	float h = 2/pixels.y;
	
	int columns = int(ceil(pixels.x) + 1);
	int row = int(v_instance[0] / columns);
	int col = int(mod(v_instance[0], columns));
	if(col >= columns)
		col = col - columns;
	
	vec4 pos = vec4(
		(col+xpos)/pixels.x *2-1 + w/2,
		(row+ypos)/pixels.y *2-1 + h/2,
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
