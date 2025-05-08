@VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform vec4 color;
out vec4 pcolor;
void main() {
	gl_Position = projection *  view * model * vec4(aPos, 1.0f);
    pcolor = color;
}


@FRAGMENT
#version 330 core
out vec4 frag_color;

uniform vec4 color;
in vec4 pcolor;

void main() {
    frag_color = pcolor;
}
