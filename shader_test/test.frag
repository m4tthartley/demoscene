#version 330

uniform sampler2D rt_tex;
in vec2 position;
out vec3 frag;

void main() {
	vec3 color = texture(rt_tex, position*0.5+0.5).rgb;
	color = pow(color, vec3(3));
	frag = color;
}