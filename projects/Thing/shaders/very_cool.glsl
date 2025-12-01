@VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform float ratio;

out vec2 tex_coord;
void main() {  

    vec4 pos = model * vec4(aPos,1.0);
    pos.x /= ratio;
	gl_Position = pos;
    tex_coord = aTex;
}



@FRAGMENT
#version 330 core
out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D texture_sample;

layout (std140) uniform Lighting{
    vec3  global_ambient;
    float global_ambient_strength;
    vec3 view_pos;
    float padding;
};

void main() {
    vec4 tmp = texture(texture_sample, tex_coord);
    tmp.xyz *= view_pos;
    float average =  0.2126 * tmp.r + 0.7152 * tmp.g + 0.0722 * tmp.b;
    frag_color = tmp;
}
