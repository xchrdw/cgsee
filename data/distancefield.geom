#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform sampler2D image;

uniform vec2 imageSize;
uniform vec2 aspect;
uniform vec2 pan;
uniform vec2 pixels;

in int v_instance[1];

out vec2 v_uv;

out float v_gray;

out flat int v_length;
out flat int v_chars[16];

// width/height ratio of the chars in the char map
const float charRatio = 32.0 / 50.0;

// Creates quad with center at 'position'
void createTextQuad(in vec2 position, in vec2 dimension, in int length, in int chars[16]) {
	vec4 pos = vec4(position, 0, 1);
	
	vec4 halfX = vec4(dimension.x/2, 0, 0, 0);
	vec4 halfY = vec4(0, dimension.y/2, 0, 0);
	
	v_length = length;
	v_chars = chars;
	
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
	
	v_gray = (pixelColor.r * 0.2126 + pixelColor.g * 0.7152 + pixelColor.b * 0.0722)/255;
	
	int chars[16] = int[16](16, // 16 = hashtag
		int(pixelColor.r/16),
		int(mod(pixelColor.r, 16)),
		int(pixelColor.g/16),
		int(mod(pixelColor.g, 16)),
		int(pixelColor.b/16),
		int(mod(pixelColor.b, 16)),
		int(pixelColor.a/16),
		int(mod(pixelColor.a, 16)), 0, 0, 0, 0, 0, 0, 0);
	
	int charCount = 9;
	
	float textHeight = h/10;
	float textWidth = w / h * textHeight * float(charCount) * charRatio;
	createTextQuad(vec2(centerX, centerY), vec2(textWidth, textHeight), charCount, chars);
}
