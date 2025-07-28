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

struct DirLight {
    vec3 direction;
    float ambient;
    vec3 color;
    float strength;
};

struct PntLight { // 3 * vec4
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float padding;
    vec3 position;
};

struct SptLight { // 5 * vec4
    vec4 position;
    vec4 color; 
    vec4 direction;
    float constant;
    float linear;
    float quadratic;

    float cutoff;

    float outer_cutoff;
    float paddin[3];
};


layout (std140) uniform SptLights {
    int spt_num;
    SptLight spts[MAX_LIGHTS];
};

layout (std140) uniform DirLights {
    DirLight dirs[MAX_LIGHTS];
    int dir_num;
};

layout (std140) uniform PntLights {
    PntLight pnts[MAX_LIGHTS];
    int pnt_num;
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


in vec2 tex_coord;
in vec3 normal;
in vec3 world_pos;

uniform sampler2D texture_sample;
uniform Material material;

vec3 norm = normalize(normal);
vec3 ambient = vec3(0.0);
vec3 diffuse = vec3(0.0);
vec3 specular = vec3(0.0);

void calc_dirs(vec3 view_dir);
void calc_pnts(vec3 view_dir);
void calc_spts(vec3 view_dir);


void main() {
    // ambient
    ambient = global_ambient * global_ambient_strength * material.ambient;

    // diffuse && specular

    vec3 view_dir = normalize(view_pos - world_pos);

    calc_dirs(view_dir);
    calc_pnts(view_dir);
    calc_spts(view_dir);

    vec3 result = (ambient + diffuse + specular);

    frag_color = vec4(result, 1.0);
}

void calc_dirs(vec3 view_dir) {

    for (int i = 0; i < int(dir_num); i++) {
        DirLight l = dirs[i];
        vec3 light_dir = normalize(-l.direction);
        ambient += l.ambient * material.ambient;



        float diff = max(dot(norm, light_dir), 0.0);
        diffuse += (l.color * l.strength) * (diff * material.diffuse);

        vec3 reflect_dir = reflect(-light_dir, norm);
        float spec = 0.0f;
        if (material.shininess != 0.0f) {
            spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        }
        specular += l.color * (spec * material.specular);
    }
}

void calc_pnts(vec3 view_dir) {

    for (int i = 0; i < int(pnt_num); i++) {
        vec3 tmp_ambient = vec3(0.0);
        vec3 tmp_diffuse = vec3(0.0);
        vec3 tmp_specular = vec3(0.0);

        PntLight l = pnts[i];
        vec3 position = l.position;

        vec3 light_dir = normalize(position - world_pos);
        tmp_ambient = l.color * material.ambient;



        float diff = max(dot(norm, light_dir), 0.0);
        tmp_diffuse = l.color * diff * material.diffuse;

        vec3 reflect_dir = reflect(-light_dir, norm);
        float spec = 0.0f;
        if (material.shininess != 0.0f) {
            spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        }
        tmp_specular = l.color * (spec * material.specular);

        float distance = length(position - world_pos);
        float attenuation = 1.0 / (l.constant + l.linear * distance + l.quadratic * (distance * distance));

        ambient  += tmp_ambient  * attenuation;
        diffuse  += tmp_diffuse  * attenuation;
        specular += tmp_specular * attenuation;
    }
}

void calc_spts(vec3 view_dir) {

    for (int i = 0; i < int(spt_num); i++) {
        vec3 tmp_ambient = vec3(0.0);
        vec3 tmp_diffuse = vec3(0.0);
        vec3 tmp_specular = vec3(0.0);

        SptLight l = spts[i];

        //l.cutoff = cos(radians(25.5));
        //l.outer_cutoff = cos(radians(38.5));

        //l.constant = 1.0f;
        //l.linear = 0.09f;
        //l.quadratic = 0.032f;
        vec3 position = l.position.xyz;

        vec3 light_dir = normalize(position - world_pos);

        //if (theta < l.cutoff) continue;
        tmp_ambient = l.color.rgb * material.ambient ;



        float diff = max(dot(norm, light_dir), 0.0);
        tmp_diffuse = l.color.rgb * diff * material.diffuse;

        vec3 reflect_dir = reflect(-light_dir, norm);
        float spec = 0.0f;
        if (material.shininess != 0.0f) {
            spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
        }
        tmp_specular = l.color.rgb * (spec * material.specular);

 
        // spotlight (soft edges)
        float theta = dot(light_dir, normalize(-l.direction.xyz)); 
        float epsilon = (l.cutoff - l.outer_cutoff);
        float intensity = clamp((theta - l.outer_cutoff) / epsilon, 0.0, 1.0);
        tmp_diffuse  *= intensity;
        tmp_specular *= intensity;

        float distance = length(position - world_pos);
        float attenuation = 1.0 / (l.constant + l.linear * distance + l.quadratic * (distance * distance));

        ambient  += tmp_ambient  * attenuation;
        diffuse  += tmp_diffuse  * attenuation;
        specular += tmp_specular * attenuation;
    }
}
