
vec3 brdf_fresnel(vec3 h, vec3 v, vec3 f0) {
	float costheta = max(dot(h, v), 0.0f);
	return f0 + ((1.0f - f0)*pow(1 - costheta, 5.0f));
}
float brdf_geometry_sub(float ndotv, float k) {
	return ndotv / (ndotv * (1.0f - k) + k);
}
float brdf_geometry(vec3 n, vec3 v, vec3 l, float roughness) {
	float ndotl = max(dot(n, l), 0.0f);
	float ndotv = max(dot(n, v), 0.0f);

	float k = (roughness + 1.0);
	k = (k*k) / 8.0;

	float ggx0 = brdf_geometry_sub(ndotv, k);
	float ggx1 = brdf_geometry_sub(ndotl, k);

	return ggx0 * ggx1;
}
float brdf_distribution(vec3 n, vec3 h, float roughness) {
	float ndoth = max(dot(n, h), 0.0f);

	float ndoth2 = ndoth*ndoth;
	float a = roughness*roughness;
	float a2 = (a*a);

	float denom = ndoth2 * (a2-1.0f) + 1.0f;

	return a2 / (PI * denom * denom);
}
vec3 light_it(vec3 pos, vec3 normal, Camera camera, Material material, vec3 reflection_color/* , const Lighting lighting */) {
	// const int num_lights = 8;
	// vec3 lights[num_lights];
	// lights[0] = vec3(10.0, 10.0, 10.0);
	// lights[1] = vec3(-10.0, 10.0, 10.0);
	// lights[2] = vec3(10.0, -10.0, 10.0);
	// lights[3] = vec3(-10.0, -10.0, 10.0);
	// lights[4] = vec3(10.0, 10.0, -10.0);
	// lights[5] = vec3(-10.0, 10.0, -10.0);
	// lights[6] = vec3(10.0, -10.0, -10.0);
	// lights[7] = vec3(-10.0, -10.0, -10.0);

	vec3 albedo = material.albedo;
	float roughness = material.roughness * (0.9) + 0.05;
	float metallic = material.metallic * (0.9) + 0.05;

	vec3 f0 = mix(vec3(0.04f), albedo, metallic);
	vec3 v = -normalize(pos - camera.pos);
	vec3 n = normalize(normal);
	float ndotv = max(dot(n, v), 0.0);

	vec3 Lo = vec3(0.0);
	for (int i = 0; i < /* num_lights */lighting.num_lights; ++i) {
		vec3 light_dir = normalize(lighting.lights[i].pos - pos);
		float light_dist = length(lighting.lights[i].pos - pos);
		vec3 radiance = vec3(30.0) /* * lights[i].color */* vec3(20.0) * (1.0 / (light_dist*light_dist));

		vec3 l = light_dir;
		vec3 h = normalize(v + l);

		float ndotl = max(dot(n, l), 0.0f);
				
		vec3 fresnel = brdf_fresnel(h, v, f0);
		vec3 kd = vec3(1.0f) - fresnel;
		kd *= 1.0f - metallic;
		vec3 lambert = albedo / PI;
			
		float D = brdf_distribution(n, h, roughness);
		vec3 F = fresnel;
		float G = (brdf_geometry(n, v, l, roughness));
		vec3 specular = (D*F*G) / ((4 * ndotv * ndotl) + 0.001);
			
		Lo += (kd*lambert + specular) * radiance * ndotl;
	}

	vec3 ambient = vec3(0.03) * albedo;
	vec3 color = ambient + Lo + ((1.0-roughness)*reflection_color);
	return color;
}
