#version 330

uniform float time;
in vec2 screen_pos;
out vec3 frag;

#include "math.glsl"
#include "scene.glsl"
#include "pbr.glsl"

SceneResult scene(vec3 pos) {
	lighting.num_lights = 1;
	lighting.lights[0].pos = vec3(5.0, 5.0, 5.0);

	//return SceneResult(cube(pos, vec3(0.0, 0.0, -15.0)), pos, Material(vec3(1.0), 1.0, 0.0));
	vec3 color = vec3(0.0, saturate(fbm(pos)), saturate(fbm(pos/4.0)));
	return SceneResult(sphere_plane(pos, 2.2, vec3(0.0, sin(time* fbm(floor(pos/2.2)))*0.25, 0.0)/* vec3(1.0, 0.201, -15.0) */), pos, Material(color, 0.0, 1.0));
}

#include "ray_march.glsl"

void main() {
	// frag = vec3(fract(time), 1.0 - fract(time), 1.0);
	// frag = vec3(1.0);

	Camera camera;// = Camera(vec3(0.0));
	camera.pos = vec3(30.0*sin(time*0.25), 10.5, 30.0*cos(time*0.25));
	STResult res = sphere_trace(screen_pos, camera);
	vec3 color;
	if (!res.hit) color = vec3(0.0);
	else color = res.color;
	//else final_color_hdr(res.color + vec3(0.0, 0.0, 1.0)*(res.depth/200.0));
	//frag = vec3(screen_pos*0.5+0.5, sin(time)*0.5+0.5);

	color = mix(color, vec3(0.2), step(40.0, res.depth)*((res.depth-40.0)/50.0) /* step(res.depth/200.0, 0.8) */);

	final_color_hdr(color);
}