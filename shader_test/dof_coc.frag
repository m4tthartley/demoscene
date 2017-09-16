#version 330

uniform sampler2D rt_tex;
uniform vec2 screen_res;
in vec2 screen_pos;
out vec3 frag;

#include "math.glsl"
#include "camera.glsl"

void main() {
	float depth = texture(rt_tex, screen_pos*0.5+0.5).a;
	float coc = get_coc_signed(depth);
	float r = min(max(coc, 0.0), 50.0);
	float g = min(abs(min(coc, 0.0)), 50.0);
	frag = vec3(r, g, 0.0) /* / 50.0 */;
}