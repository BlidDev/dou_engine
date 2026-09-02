@VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;



uniform vec3 position;
uniform vec3 color;
uniform float time;
out vec2 tex_coord;
out vec3 f_color;


void main() {
    tex_coord = aTex;
    f_color = color;
	gl_Position = vec4(aPos + position, 1.0f) * 0.5f + sin(time);
}


@FRAGMENT
#version 330 core
out vec4 frag_color;


in vec2 tex_coord;
in vec3 f_color;

void main() {
    frag_color = vec4(f_color, 1.0f);
}
