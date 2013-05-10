#version 150

in float edge_dist[3];

out vec4 fragColor;

void main(void) {
    float dist = min(edge_dist[0], min(edge_dist[1], edge_dist[2]));

    if(dist > 0.01)
        discard;

    float m = dist*50;

    fragColor = vec4(m, m + 0.7, m, 1.0);
}
