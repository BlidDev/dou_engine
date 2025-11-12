@VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
void main() {
	gl_Position = projection *  view * model * vec4(aPos, 1.0f);
}


@FRAGMENT
#version 330 core
out vec4 frag_color;

uniform vec3 id_color;

void main() {
    frag_color = vec4(id_color,1.0f);
}
