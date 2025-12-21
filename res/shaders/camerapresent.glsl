@VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

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

void main() {
    frag_color = texture(texture_sample, tex_coord);
}
