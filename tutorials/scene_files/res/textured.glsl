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
	gl_Position = projection *  view * model * vec4(aPos, 1.0f);
    tex_coord = aTex;
}



@FRAGMENT
#version 330 core
out vec4 frag_color;

in vec2 tex_coord;

struct Material {
    vec2 tex_repeat;
};

uniform sampler2D texture_sample;
uniform Material material;

void main() {
    frag_color = texture(texture_sample, tex_coord * material.tex_repeat);
}
