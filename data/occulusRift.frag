#version 150 core

in vec4 gl_FragCoord;
out vec4 fragColor;

uniform sampler2D leftTexture;
uniform sampler2D rightTexture;
uniform vec2 viewport;


void main()
{
    vec2 uvLeft = vec2(2*gl_FragCoord.x/viewport.x,gl_FragCoord.y/viewport.y);
    vec2 uvRight = vec2(2*(gl_FragCoord.x-viewport.x/2)/viewport.x,gl_FragCoord.y/viewport.y);
    if(gl_FragCoord.x < viewport.x/2){
        fragColor = texture(leftTexture, uvLeft);
    }
    else{
        fragColor = texture(rightTexture, uvRight);
    }
    //fragColor = vec4(1.0);
}
