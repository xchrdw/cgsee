// #version 410
#version 150 core

in vec2 a_vertex;
// in vec3 a_normal;

// uniform mat4 transform;
uniform mat4 transformInverse;
// uniform mat4 view;
uniform vec3 cameraposition;

// out vec3 normal;
// out vec3 position;
// out vec3 objCoord;

out vec2 v_uv;
out vec3 direction;

void main(void) 
{
    vec4 worldPos = transformInverse * vec4(a_vertex, -1.0, 1.0);
    // cam (0, 0, 1.0)
    // pos (x, y, z)
    // dir (x, y, z-1.0)
    direction = worldPos.xyz - cameraposition;

    // vec4 dir = vec4(a_vertex, -1.0, 0.0);
    // direction = (dir / dir.w).xyz;
    // direction = dir.xyz;
    
	v_uv = a_vertex.xy * 0.5 + 0.5;
	gl_Position = vec4(a_vertex, 0.0, 1.0);
    /* normal = normalize(a_normal);
    position = (view * vec4(a_vertex, 1.0)).xyz;
    objCoord = a_vertex.xyz;
    gl_Position = transform * vec4(a_vertex, 1.0); */
}
