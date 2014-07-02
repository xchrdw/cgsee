#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 28) out;

uniform sampler2D image;

uniform vec2 imageSize;
uniform vec2 aspect;
uniform vec2 pan;
uniform vec2 pixels;

in int v_instance[1];

out vec2 v_uv;
out flat int v_char;

// Creates quad with center at 'position'
void createTextQuad(in vec2 position, in vec2 dimension, in int character) {
	vec4 pos = vec4(position, 0, 1);
	
	vec4 halfX = vec4(dimension.x/2, 0, 0, 0);
	vec4 halfY = vec4(0, dimension.y/2, 0, 0);
	v_char = character;
	
	gl_Position = pos + halfX - halfY;
	v_uv = vec2(1,0);
	EmitVertex();
	gl_Position = pos + halfX + halfY;
	v_uv = vec2(1,1);
	EmitVertex();
	gl_Position = pos - halfX - halfY;
	v_uv = vec2(0,0);
	EmitVertex();
	gl_Position = pos - halfX + halfY;
	v_uv = vec2(0,1);
	EmitVertex();
    EndPrimitive();
}


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
	
	float centerX = (col+xpos)/pixels.x *2-1 + w/2;
	float centerY = (row+ypos)/pixels.y *2-1 + h/2;

	float currentX = centerX - w/4;
	
	vec4 pixelColor = texture(image, vec2(col, row)/imageSize + pan/aspect) * 255;
	
	createTextQuad(vec2(currentX, centerY), vec2(w, h)/12, 16); // 16 = hashtag
	currentX += w/12;
	createTextQuad(vec2(currentX, centerY), vec2(w, h)/12, int(pixelColor.r/16));
	currentX += w/12;
	createTextQuad(vec2(currentX, centerY), vec2(w, h)/12, int(mod(pixelColor.r, 16)));
	currentX += w/12;
	createTextQuad(vec2(currentX, centerY), vec2(w, h)/12, int(pixelColor.g/16));
	currentX += w/12;
	createTextQuad(vec2(currentX, centerY), vec2(w, h)/12, int(mod(pixelColor.g, 16)));
	currentX += w/12;
	createTextQuad(vec2(currentX, centerY), vec2(w, h)/12, int(pixelColor.b/16));
	currentX += w/12;
	createTextQuad(vec2(currentX, centerY), vec2(w, h)/12, int(mod(pixelColor.b, 16)));
}
