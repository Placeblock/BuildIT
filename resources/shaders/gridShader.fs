#version 330

out vec4 FragColor;

uniform vec2 offset;
uniform vec2 resolution;
uniform float zoom;

void main() {
    float cellSize = 32.0 * zoom;
    vec2 fragCoord = vec2(gl_FragCoord.xy) - cellSize/2;
    fragCoord.x += offset.x * zoom;
    fragCoord.y -= offset.y * zoom;
    fragCoord.y = resolution.y-fragCoord.y;
    vec2 circleDelta = mod(fragCoord, cellSize);
    float dist = distance(circleDelta, vec2(cellSize/2, cellSize/2));
    if (dist < 4.5*zoom) {
        FragColor = vec4(0.15, 0.15, 0.15, 1.0);
    } else if (dist < 5.5*zoom) {
        FragColor = vec4(0.3, 0.3, 0.3, 1.0);
    } else {
        FragColor = vec4(0.1, 0.1, 0.1, 1.0);
    }
}