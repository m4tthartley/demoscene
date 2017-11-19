#version 330

uniform sampler2D rt_tex;
uniform sampler2D rt_coc;
uniform vec2 screen_res;
in vec2 screen_pos;
out vec4 farfrag;
out vec4 nearfrag;

#include "math.glsl"
#include "camera.glsl"

void main() {
	vec3 sample = texture(rt_tex, screen_pos*0.5+0.5).rgb;
	vec4 coc = texture(rt_coc, screen_pos*0.5+0.5);
	if (coc.r > 0.0) {
		farfrag = vec4(sample * (coc.r/50.0), coc.r);
		nearfrag = vec4(sample, 0.0);
	}
	if (coc.g > 0.0) {
		farfrag = vec4(0.0);
		nearfrag = vec4(sample * (coc.g/50.0), coc.g);
	}
}