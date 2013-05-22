#version 150 core

in vec4 gl_FragCoord;
out vec4 fragcolor;
out vec4 fragcolor2;

in vec3 v_normal;

uniform float znear;
uniform float zfar;

void main()
{
        vec3 n = normalize(v_normal);
        n *= 0.5;
        n += 0.5;

        float z = gl_FragCoord.z;

        // d = (2.0 * zfar * znear / (zfar + znear - (zfar - znear) * (2.0 * z- 1.0)));
        // normalized to [0,1]
        // d = (d - znear) / (zfar - znear);

        // simplyfied with wolfram alpha
        z = - znear * z / (zfar * z - zfar - znear * z);

        fragcolor = vec4(n, z);
		fragcolor2 = vec4(0.5);
}
