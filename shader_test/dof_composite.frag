#version 330

uniform sampler2D rt_far;
uniform sampler2D rt_near;
uniform sampler2D rt_main;
uniform sampler2D rt_coc;
uniform vec2 screen_res;
in vec2 screen_pos;
out vec4 frag;

#include "math.glsl"
#include "camera.glsl"

void main() {
	// frag = vec4(1.0, 1.0, 0.0, 1.0);
	vec3 infocus = texture(rt_main, screen_pos*0.5+0.5).rgb;
	vec3 far = texture(rt_far, screen_pos*0.5+0.5).rgb;
	vec3 near = texture(rt_near, screen_pos*0.5+0.5).rgb;
	vec4 coc = texture(rt_coc, screen_pos*0.5+0.5) / 50.0;
	float coc_far = coc.r;
	float coc_near = texture(rt_near, screen_pos*0.5+0.5).a/50.0;

	vec3 color = vec3(0.0);
	color = mix(infocus, far, vec3(texture(rt_coc, screen_pos*0.5+0.5).r / 50.0));
	color = mix(color, near, texture(rt_near, screen_pos*0.5+0.5).a/50.0);

	// if (coc_far > 0.1) color = far;
	// if (coc_near > 0.1) color = near;

	// color = near;

	// color = vec3(mix(infocus.r, far.r, 0.0), 0.0, 0.0);
	// color = vec3((infocus.r + (far.r - infocus.r) * 0.5), 0.0, 0.0);

	// color = vec3(infocus.r, 0.0, 0.0);
	// vec3 color = mix(infocus, near, texture(rt_coc, screen_pos*0.5+0.5).g / 50.0);
	// frag = far;
	frag = vec4(color, 1.0);
}