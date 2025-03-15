@VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
	gl_Position = projection *  model * vec4(aPos, 1.0f);
    }
@FRAGMENT
#version 330 core
out vec4 frag_color;

uniform vec4 color;

void main() {
    frag_color = color;
}
