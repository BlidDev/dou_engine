
@VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

out vec3 coord;
out vec2 tex_coord;
void main() {
	gl_Position = projection *  view * model * vec4(aPos, 1.0f);
    tex_coord = aTex;
    coord = aPos;
}



@FRAGMENT
#version 330 core
out vec4 frag_color;

in vec2 tex_coord;
in vec3 coord;

uniform sampler2D texture_sample;

void main() {
    float dis = length(coord - vec3(0));
    frag_color = (vec4(1,0,1,1) * dis) + texture(texture_sample, tex_coord);
}
