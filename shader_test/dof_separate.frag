#version 330

uniform sampler2D rt_tex;
uniform vec2 screen_res;
in vec2 screen_pos;
out vec4 frag;
out vec4 frag2;

#include "math.glsl"
#include "camera.glsl"

void main() {
	vec4 sample = texture(rt_tex, screen_pos*0.5+0.5);
	if (sample.a-1.0 > focus) {
		frag = sample;
		frag2 = sample;
	} else {
		frag = vec4(0.0);
		frag2 = sample;
	}
}