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
	lighting.num_lights = 4;
	lighting.lights[0].pos = vec3(5.0, 5.0, 5.0);
	lighting.lights[0].color = vec3(10.0, 0.0, 10.0);
	lighting.lights[1].pos = vec3(-5.0, 5.0, -5.0);
	lighting.lights[1].color = vec3(0.0, 5.0, 5.0);
	lighting.lights[2].pos = vec3(-50.0, 5.0, 40.0);
	lighting.lights[2].color = vec3(5.0, 5.0, 5.0);
	lighting.lights[3].pos = vec3(50.0, 5.0, -40.0);
	lighting.lights[3].color = vec3(5.0, 5.0, 5.0);

	// float a = cube(pos);
	// for (int y = 0; y < 10; ++y)
	// for (int x = 0; x < 10; ++x) {
	// 	vec3 off = vec3(-(3.0*5.0) + 3.0 + 3.0*x, 0.0, -(3.0*5.0) + 3.0 + 3.0*y);
	// 	a = union(a, cube(yrotate(pos, 0.0), off));
	// }
	// todo: I think my cubes are messed up
	float a = cube_plane(pos, 2.2/* 4.2 */); // todo: large spacing causes issues
	SceneResult cube_floor = SceneResult(a, pos, Material(vec3(1.0), 0.0, 0.0));

	a = cube(pos, vec3(-50.0, 5.0, -50.0), vec3(0.25));
	a = union(a, cube(pos, vec3( 50.0, 5.0, -50.0), vec3(0.25)));
	a = union(a, cube(pos, vec3( 50.0, 5.0,  50.0), vec3(0.25)));
	a = union(a, cube(pos, vec3(-50.0, 5.0,  50.0), vec3(0.25)));
	SceneResult floating_cubes = SceneResult(a, pos, Material(vec3(15.0, 15.0, 15.0), 0.0, 0.0));
	return sr_union(cube_floor, floating_cubes);
}

#include "ray_march.glsl"

void main() {
	Camera camera;
	camera.pos = vec3(25.0*sin(time*0.2), 8.0, 25.0*cos(time*0.2));
	STResult res = sphere_trace(screen_pos, camera);
	frag = vec4(gamma_hdr(res.color), res.depth);
	// if (abs(screen_pos.x)<0.02 && abs(screen_pos.y)<0.035) {
	// 	frag = vec4(1.0, 0.0, 0.0, 80.0);
	// } else {
	// 	frag = vec4(0.0, 0.0, 0.0, 100.0);
	// }
}