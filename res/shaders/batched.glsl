@VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 3) in vec3 aModel;
layout (location = 4) in vec3 aColor;



out vec2 tex_coord;
out vec3 color;

void main() {
    tex_coord = aTex;
    color = aColor;
	gl_Position = vec4((aPos * 0.1f) + aModel, 1.0f);
}


@FRAGMENT
#version 330 core
out vec4 frag_color;


in vec3 color;
in vec2 tex_coord;

void main() {
    frag_color = vec4(color, 1.0f);
}
