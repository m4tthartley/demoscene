#version 330

uniform float time;

#include "math.glsl"

// smoothstep = f*f*(3.0-2.0*f)

in vec2 screen_pos;
out vec3 frag;

//float aspect = 1280.0/720.0;
//float t = time*0.25;
//vec2 p = vec2(position.x*-aspect, position.y);

// vec3 yrotate(vec3 x, float a) {
// 	return vec3(x.x*cos(a) - x.z*sin(a), x.y, x.z*cos(a) + x.x*sin(a));
// }

#include "scene.glsl"
#include "pbr.glsl"

SceneResult scene(vec3 pos) {
	lighting.num_lights = 1;//int(floor(mod(time, 8.0)));
	lighting.lights[0].pos = vec3(15.0, 4.0, 15.0);
	// lighting.lights[1].pos = vec3(-10.0, 10.0, 10.0);
	// lighting.lights[2].pos = vec3(10.0, -10.0, 10.0);
	// lighting.lights[3].pos = vec3(-10.0, -10.0, 10.0);
	// lighting.lights[4].pos = vec3(10.0, 10.0, -10.0);
	// lighting.lights[5].pos = vec3(-10.0, 10.0, -10.0);
	// lighting.lights[6].pos = vec3(10.0, -10.0, -10.0);
	// lighting.lights[7].pos = vec3(-10.0, -10.0, -10.0);

	// float a = sphere(pos, vec3(0.0, 0.0, 0.0), 1.4);
	// a = difference(cube(pos), a);
	// a = union(a, intersect(sphere(pos, vec3(0.0), 0.27), cube(yrotate(pos, time*2.0), vec3(0.0), vec3(0.2))));
	// return a;

	float y = clamp(pos.y*(0.5*(sin(fract(time*0.5)*PI2))), -1.0, 1.0);
	float s = sin(y);
	float c = cos(y);
	vec3 p = vec3(pos.x*c - pos.z*s, pos.y, pos.z*c + pos.x*s);
	// vec3 p = yrotate(pos, pos.y*0.5);

	float spacing = 2.0;
	// return sphere_plane(pos, spacing, vec3(/* 1.0*sin(time) */0.0, 4.0*cos(1.0*time+noise(floor(vec3(pos.x/spacing, pos.z/spacing, 0.0))*spacing)), 0.0));
	float o = sin(fbm(vec3(vec2(1.0, 1.0), time*0.25))*PI2);
	vec3 stuff[8];
	stuff[0] = vec3(-1.5, -1.5, -1.5);
	stuff[1] = vec3( 1.5, -1.5, -1.5);
	stuff[2] = vec3(-1.5,  1.5, -1.5);
	stuff[3] = vec3( 1.5,  1.5, -1.5);
	stuff[4] = vec3(-1.5, -1.5, 1.5);
	stuff[5] = vec3( 1.5, -1.5, 1.5);
	stuff[6] = vec3(-1.5,  1.5, 1.5);
	stuff[7] = vec3( 1.5,  1.5, 1.5);
	float a = sphere(pos, 			stuff[0]*vec3(sin(fbm(stuff[0])+time), sin(fbm(stuff[0])+time), sin(fbm(stuff[0])+time)));
	a = smooth_union(a, sphere(pos, stuff[1]*vec3(sin(fbm(stuff[1])+time*1.1), sin(fbm(stuff[1])+time*1.1), sin(fbm(stuff[1])+time*1.1))));
	a = smooth_union(a, sphere(pos, stuff[2]*vec3(sin(fbm(stuff[2])+time*1.2), sin(fbm(stuff[2])+time*1.2), sin(fbm(stuff[2])+time*1.2))));
	a = smooth_union(a, sphere(pos, stuff[3]*vec3(sin(fbm(stuff[3])+time*1.3), sin(fbm(stuff[3])+time*1.3), sin(fbm(stuff[3])+time*1.3))));
	a = smooth_union(a, sphere(pos, stuff[4]*vec3(sin(fbm(stuff[4])+time*1.4), sin(fbm(stuff[4])+time*1.4), sin(fbm(stuff[4])+time*1.4))));
	a = smooth_union(a, sphere(pos, stuff[5]*vec3(sin(fbm(stuff[5])+time*1.5), sin(fbm(stuff[5])+time*1.5), sin(fbm(stuff[5])+time*1.5))));
	a = smooth_union(a, sphere(pos, stuff[6]*vec3(sin(fbm(stuff[6])+time*1.6), sin(fbm(stuff[6])+time*1.6), sin(fbm(stuff[6])+time*1.6))));
	a = smooth_union(a, sphere(pos, stuff[7]*vec3(sin(fbm(stuff[7])+time*1.7), sin(fbm(stuff[7])+time*1.7), sin(fbm(stuff[7])+time*1.7))));
	SceneResult res = SceneResult(a, pos, /* Material(vec3(0.2, 0.1, 1.0), 0.4, 0.7) */Material(vec3(1.0, 1.0, 1.0), 0.0, 1.0));

	Material m = Material(vec3(1.0, 1.0, 1.0), 0.0, 0.0);
	Material sand = Material(vec3(0.2, 0.2, 0.05), 1.0, 0.0);
	res = sr_union(res, SceneResult(plane(pos, vec4(-1.0, 0.0, 0.0, 20.0)), pos, Material(vec3(1.0, 0.0, 0.0), 0.0, 0.0)));
	res = sr_union(res, SceneResult(plane(pos, vec4(1.0, 0.0, 0.0, 20.0)), pos, Material(vec3(0.0, 1.0, 0.0), 0.0, 0.0)));
	res = sr_union(res, SceneResult(plane(pos, vec4(0.0, -1.0, 0.0, 5.0 /* - noise(pos)*0.1 */)), pos, Material(vec3(1.0), 0.5, 0.0)));
	res = sr_union(res, SceneResult(plane(pos, vec4(0.0, 1.0, 0.0, 5.0)), pos, m));
	res = sr_union(res, SceneResult(plane(pos, vec4(0.0, 0.0, -1.0, 20.0)), pos, Material(vec3(0.0, 0.0, 1.0), 0.0, 0.0)));
	res = sr_union(res, SceneResult(plane(pos, vec4(0.0, 0.0, 1.0, 20.0)), pos, Material(vec3(1.0, 0.0, 1.0), 0.0, 0.0)));
	return res;

	// return cube(p, vec3(1.0*sin(time), 1.0*cos(time), 0.0));
}

#include "ray_march.glsl"

void main() {
	
#if 1
	

	{
		float t = time;
		Camera camera;
		camera.pos = vec3(sin(t)*10.0, sin(fract(time*0.25)*PI2)*4.0, cos(t)*10.0);
		// camera.pos = vec3(sin(t)*10.0, 0.0, cos(t)*10.0);
		// camera.pos = vec3(sin(t)*20.0, sin(fract(time*0.1)*PI2)*10.0, cos(t)*20.0);
		// camera.pos = vec3(0.0, 0.0, 10.0);

		
		// ray_dir.x *= -normalize(camera).x;
		// ray_dir.z *= -normalize(camera).z;

		// float dist = ray_march(vec3(0.0, 0.0, 10.0), ray_dir);
		
		STResult res = sphere_trace(screen_pos, camera);

		// float modulate = 1.0;
		// vec3 n = vec3(pow(fbm(pos)*modulate, 1.0),
		// 			  pow(fbm(pos+10)*modulate, 1.0),
		// 			  pow(fbm(pos+20)*modulate, 1.0));
		// normal *= n;
		// normal = normalize(normal);

		if (!res.hit) {
			final_color(vec3(0.001, 0.001, 0.01));
		} else {
//			final_color(vec3(1.0, fract(time), 1.0-fract(time)));
			
			
//			final_color_hdr(light_it(pos, normal, camera, res.mat, reflect_color));
			// out_color = vec4(light_it(pos, normal, Camera(camera, cam_mat)), 1.0);
		}
	}

// 	#if 1
// 			f = saturate(pow((fbm(pos*2.0)), 30.0));
// #else
// 			f = saturate(pow(fbm(pos * 10.0 * fbm(normal * 1.0)), 2.0) + 0.0);
// #endif

/* REAL NICE
			Material m1 = Material(vec3(0.01, 0.01, 0.01), 0.0, 0.0);
			Material m2 = Material(vec3(0.0, 0.1, 0.3), 0.8, 0.2);
			Material m = Material(mix(m1.albedo, m2.albedo, f), mix(m1.roughness, m2.roughness, f), mix(m1.metallic, m2.metallic, f)); */
#endif
}
