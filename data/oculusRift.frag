#version 150 core

in vec4 gl_FragCoord;
in vec2 v_uv;
out vec4 fragColor;

uniform ivec2 viewport;
uniform sampler2D leftCam;
uniform sampler2D rightCam;

vec4 HmdWarpParam = vec4(1.0,0.22,0.24,0.0);
vec2 ScaleIn = vec2(4.0,2.0);
vec2 Scale2 = vec2(0.125,0.5);

vec2 HmdWarp(vec2 in01, vec2 LensCenter, vec2 Scale)
{
    //vec2 LensCenter = vec2(0.25,0.5);
    vec2 theta = (in01 - LensCenter) * ScaleIn; // Scales to [-1, 1]
    float rSq = theta.x * theta.x + theta.y * theta.y;
    vec2 rvector= theta * ( HmdWarpParam.x + HmdWarpParam.y * rSq +
        HmdWarpParam.z * rSq * rSq
        + HmdWarpParam.w * rSq * rSq * rSq);
    return (LensCenter + Scale * rvector);
}


void main()
{
    //fragColor = texture(leftCam, v_uv);
    
    vec2 uvLeft = vec2(2*v_uv.x,v_uv.y);
    vec2 uvRight = vec2(2*(v_uv.x-0.5),v_uv.y);
    
    if(v_uv.x < 0.5){
        vec2 ScreenCenter = vec2(0.25,0.5);
        vec2 warp = HmdWarp(v_uv, ScreenCenter,vec2(0.125,0.5));
        if (all(equal(clamp(warp, ScreenCenter-vec2(0.125,0.5), ScreenCenter+vec2(0.125,0.5)), warp)))
        {
           
            fragColor = mix( texture(leftCam, HmdWarp(uvLeft,vec2(0.5), vec2(0.125,0.5))) , vec4(0.0), smoothstep(0.45 , 0.5, distance(warp,ScreenCenter)) ); 
            //texture(leftCam, HmdWarp(uvLeft,vec2(0.5),vec2(0.125,0.5)));
            //fragColor = mix(tmpColor,vec4(0.0), smoothstep(0.45 , 0.5, distance(vec2(warp.y, warp.x),ScreenCenter)));
        }
        else
        {
            fragColor=vec4(0.0);
        }
    }
    else{
        vec2 ScreenCenter = vec2(0.75,0.5);
        vec2 warp = HmdWarp(v_uv,ScreenCenter,vec2(0.125,0.5));
        if (all(equal(clamp(warp, ScreenCenter-vec2(0.125,0.5), ScreenCenter+vec2(0.125,0.5)), warp)))
        {
            fragColor = mix( texture(rightCam, HmdWarp(uvRight,vec2(0.5), vec2(0.125,0.5))) , vec4(0.0), smoothstep(0.45 , 0.5, distance(warp,ScreenCenter)) );
        }
        else
        {
            fragColor=vec4(0.0);
        }
    }
    
}


/*
    vec2 uvLeft = vec2(2*v_uv.x/viewport.x,v_uv.y/viewport.y);
    vec2 uvRight = vec2(2*(v_uv.x-viewport.x/2)/viewport.x,v_uv.y/viewport.y);
    */