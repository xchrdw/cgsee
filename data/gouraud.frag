#version 330


//partly from http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_4.pdf

in vec4 gl_FragCoord;
out vec4 gl_FragColor;
in vec4 color;

void main()
{
	gl_FragColor=color;
}
