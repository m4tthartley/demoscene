#version 330

uniform float time;
in vec2 screen_pos;
out vec4 frag;

#line 0 "math.glsl"
#include "math.glsl"
#line 0 "scene.glsl"
#include "scene.glsl"
#line 0 "pbr.glsl"
#include "pbr.glsl"
#line 13 "cubes.frag"

SceneResult scene(vec3 pos) {
	lighting.num_lights = 2;
	lighting.lights[0].pos = vec3(5.0, 5.0, 5.0);
	lighting.lights[0].color = vec3(10.0, 0.0, 10.0);
	lighting.lights[1].pos = vec3(-5.0, 5.0, -5.0);
	lighting.lights[1].color = vec3(0.0, 5.0, 5.0);

	// float a = cube(pos);
	// for (int y = 0; y < 10; ++y)
	// for (int x = 0; x < 10; ++x) {
	// 	vec3 off = vec3(-(3.0*5.0) + 3.0 + 3.0*x, 0.0, -(3.0*5.0) + 3.0 + 3.0*y);
	// 	a = union(a, cube(yrotate(pos, 0.0), off));
	// }
	float a = cube_plane(pos, 4.2);
	return SceneResult(a, pos, Material(vec3(1.0), 0.0, 0.0));
}

#include "ray_march.glsl"

void main() {
	Camera camera;
	camera.pos = vec3(30.0*sin(time*0.2), 10.0, 30.0*cos(time*0.2));
	STResult res = sphere_trace(screen_pos, camera);
	frag = vec4(gamma_hdr(res.color), res.depth);
}