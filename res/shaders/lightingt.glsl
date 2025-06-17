@VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNor;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};


uniform mat4 model;
uniform mat3 normal_mat; 

out vec2 tex_coord;
out vec3 normal;
out vec3 world_pos;

void main() {
    tex_coord = aTex;
    normal = normal_mat * aNor;
    world_pos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection *  view * model * vec4(aPos, 1.0);
}


//===============================================================
@FRAGMENT
//===============================================================

#version 330 core
out vec4 frag_color;

const int MAX_LIGHTS = 32;

struct Light {
    vec3 position;
    float padding;
    vec3 color;
    float strength;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


layout (std140) uniform Lighting{
    vec3  global_ambient;
    float global_ambient_strength;
    vec3 view_pos;
    float padding;
};

layout (std140) uniform SceneLights{
    Light lights[MAX_LIGHTS];
    int lights_num;
};

in vec2 tex_coord;
in vec3 normal;
in vec3 world_pos;

uniform sampler2D texture_sample;
uniform Material material;

void main() {
    // ambient
    vec3 final_ambient = global_ambient * global_ambient_strength * material.ambient;

    // diffuse && specular
    vec3 norm = normalize(normal);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 view_dir = normalize(view_pos - world_pos);

    for (int i = 0; i < lights_num; i++) {
        Light l = lights[i];
        vec3 light_dir = normalize(l.position - world_pos);


        float diff = max(dot(norm, light_dir), 0.0);
        diffuse += (l.color * l.strength) * (diff * material.diffuse);

        vec3 reflect_dir = reflect(-light_dir, norm);
        float spec = 0.0f;
        if (material.shininess != 0.0f) {
            spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        }
        specular += l.color * (spec * material.specular);
    }
    vec3 result = (final_ambient + diffuse + specular);

    frag_color = vec4(result, 1.0) * texture(texture_sample, tex_coord);
}
