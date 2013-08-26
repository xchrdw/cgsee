#version 150 core

in vec4 gl_FragCoord;
in vec2 v_uv;
out vec4 fragColor;

uniform vec2 viewport;
uniform sampler2D leftCam;
uniform sampler2D rightCam;


void main()
{
    //fragColor = texture(leftCam, v_uv);
    
    vec2 uvLeft = vec2(2*v_uv.x,v_uv.y);
    vec2 uvRight = vec2(2*(v_uv.x-0.5),v_uv.y);
    if(v_uv.x < 0.5/*viewport.x/2*/){
        fragColor = texture(leftCam, uvLeft);
    }
    else{
        fragColor = texture(rightCam, uvRight);
    }
}


/*
    vec2 uvLeft = vec2(2*v_uv.x/viewport.x,v_uv.y/viewport.y);
    vec2 uvRight = vec2(2*(v_uv.x-viewport.x/2)/viewport.x,v_uv.y/viewport.y);
    */