#version 150

varying float edge_dist[3];

void main(void) {
    float dist = min(edge_dist[0], min(edge_dist[1], edge_dist[2]));

    if(dist > 0.01)
        discard;

    float m = dist*50;

    gl_FragColor = vec4(m, m + 0.7, m, 1.0);
}
