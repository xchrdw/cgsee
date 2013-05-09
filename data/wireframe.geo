#version 150

#define M_PI 3.1415926535897932384626433832795

in vec3 v_normal[];
varying float edge_dist[3];

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void _EmitVertex(vec4 position, float d1, float d2, float d3) {
    gl_Position = position;
    edge_dist[0] = d1;
    edge_dist[1] = d2;
    edge_dist[2] = d3;
    EmitVertex();
}

float dist(vec4 v1, vec4 v2, vec4 v3) {
    vec4 d1, d2;
    float alpha, beta;

    d1 = v1 - v3;
    d2 = v2 - v3;

    alpha = acos(dot(d1, d2) / (length(d1) * length(d2)));
    alpha = min(alpha, M_PI - alpha);
    beta = M_PI/2 - alpha;

    return abs(cos(beta) * length(d1));
}

void main(void) {
    _EmitVertex(gl_PositionIn[0], dist(gl_PositionIn[0], gl_PositionIn[1], gl_PositionIn[2]), 0, 0);
    _EmitVertex(gl_PositionIn[1], 0, dist(gl_PositionIn[1], gl_PositionIn[0], gl_PositionIn[2]), 0);
    _EmitVertex(gl_PositionIn[2], 0, 0, dist(gl_PositionIn[2], gl_PositionIn[0], gl_PositionIn[1]));

    EndPrimitive();
}
