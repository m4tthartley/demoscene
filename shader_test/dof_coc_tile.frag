#version 330

uniform sampler2D rt_tex;
uniform vec2 screen_res;
uniform vec2 coc_tile_res;
in vec2 screen_pos;
out vec3 frag;

#include "math.glsl"
#include "camera.glsl"

void main() {
	// float depth = texture(rt_tex, screen_pos*0.5+0.5).a;
	// float coc = get_coc_signed(depth);

	vec2 size = vec2(1.0) / coc_tile_res;

	vec2 res = vec2(0.0);

	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			// vec2 d = vec2(vec2(-1.0+(x*(1.0/3.5)), -1.0+(y*(1.0/3.5))));
			// vec2 coord = (screen_pos*0.5+0.5) + (d*(pix/2.0)*disc);
			
			vec2 pos = (screen_pos*0.5+0.5) + -vec2(size*0.5) + (vec2(x, y)/8.0 * size);
			res = max(res, texture(rt_tex, pos).rg);
		}
	}

	if (res.r > res.g) res.g = 0.0;
	else res.r = 0.0;

	// float r = texture(rt_tex, screen_pos*0.5+0.5).r;
	// float g = texture(rt_tex, screen_pos*0.5+0.5).g;

	frag = vec3(res /* /30.0 */ , 0.0) /* / 30.0 */;

	// float r = max(coc, 0.0);
	// float g = abs(min(coc, 0.0));
	// frag = vec3(r/* /30.0 */, g/* /30.0 */, 0.0);
}