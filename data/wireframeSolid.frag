#version 150

in float edge_dist[3];

out vec4 fragColor;

void main(void) {
    float dist = min(edge_dist[0], min(edge_dist[1], edge_dist[2]));
    fragColor = vec4(dist*19, dist*7, 0,  1.0);
}
