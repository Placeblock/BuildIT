#version 450

layout (location = 0) out vec4 outColor;

void main() {
    vec2 offset = vec2(0, 0);
    float zoom = 1.0;

    float cellSize = 32.0 * zoom;
    vec2 fragCoord = vec2(gl_FragCoord.xy) - cellSize / 2;
    fragCoord.x += offset.x * zoom;
    fragCoord.y -= offset.y * zoom;
    vec2 circleDelta = mod(fragCoord, cellSize);
    float dist = distance(circleDelta, vec2(cellSize / 2, cellSize / 2));
    if (dist < 4.5 * zoom) {
        outColor = vec4(0.05, 0.05, 0.05, 1.0);
    } else if (dist < 5.5 * zoom) {
        outColor = vec4(0.08, 0.08, 0.08, 1.0);
    } else {
        outColor = vec4(0.03, 0.03, 0.03, 1.0);
    }
}