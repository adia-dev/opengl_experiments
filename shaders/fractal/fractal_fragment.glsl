#version 330 core

out vec4 FragColor;
in vec2 frag_position;

uniform float scale;
uniform float aspect_ratio;
uniform vec2 displacement;
uniform vec2 c, z;

float squared_magnitude(vec2 v);
float generate(vec2 z, vec2 c, float max_iterations);
vec3 color(float t);
vec3 simple_color(float t);

void main() {
    vec2 transformed_position = (frag_position / scale) + displacement;
    transformed_position.x *= aspect_ratio;

    vec2 local_z = transformed_position;
    vec2 local_c = c;

    float iterations = generate(local_z, local_c, 50);
    FragColor = vec4(simple_color(iterations), 1.0f);
}

float squared_magnitude(vec2 v) {
    return v.x * v.x + v.y * v.y;
}

vec3 color(float t) {
    float frequency = 0.3;
    float amplitude = 0.5;
    float center = 0.5;

    float red = sin(frequency * t + 0.3) * amplitude + center;
    float green = sin(frequency * t + 1.0) * amplitude + center;
    float blue = sin(frequency * t + 1.2) * amplitude + center;

    return vec3(clamp(red, 0.0, 1.0), clamp(green, 0.0, 1.0), clamp(blue, 0.0, 1.0));
}

vec3 simple_color(float t) {
    float intensity = t;
    return vec3(intensity, intensity, intensity);
}


float generate(vec2 z, vec2 c, float max_iterations)
{
    int i = 0;

    while(i < max_iterations && squared_magnitude(z) < 4.0) {
        float temp = z.x * z.x - z.y * z.y + c.x;
        z.y = 2.0 * z.x * z.y + c.y;
        z.x = temp;
        i++;
    }

    return (float(i) - log2(log2(squared_magnitude(z)))) / (float(max_iterations));
}
