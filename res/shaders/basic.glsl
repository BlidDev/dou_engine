@VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

out vec2 tex_coord;

void main() {
    tex_coord = aTex;
	gl_Position = projection *  view * model * vec4(aPos, 1.0f);
}


@FRAGMENT
#version 330 core
out vec4 frag_color;


struct Material {
    vec3 ambient;
    vec2 tex_repeat;
    bool is_textured;
};

in vec2 tex_coord;

uniform sampler2D texture_sample;
uniform Material material;

void main() {
    frag_color = vec4(material.ambient, 1.0f);
    if(material.is_textured)
        frag_color *= texture(texture_sample, tex_coord * material.tex_repeat);
}
