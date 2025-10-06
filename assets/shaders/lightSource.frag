#version 460 core

out vec4 FragColor;
struct Light {
// vec3 position; // not needed since we are doing specular in the view projection

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

void main(){
    FragColor = vec4(light.specular, 1.0);
}