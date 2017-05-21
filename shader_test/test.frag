#version 330

uniform sampler2D rt_tex;
in vec2 screen_pos;
out vec3 frag;

void main() {
	vec3 color = texture(rt_tex, screen_pos*0.5+0.5).rgb;
	//color = pow(color, vec3(1.5));
	//color.b *= 0.6;
	frag = color;
}