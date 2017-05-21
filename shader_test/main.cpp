
#include "w:/libs/rain.c"
#include <GL/glu.h>
#include "w:/libs/gfx.c"
#pragma comment(lib, "opengl32.lib")

int2 gfx_size = {1920, 1080};
float2 gfx_quad_verts[] = {
	-1.0, -1.0,
	 1.0, -1.0,
	 1.0,  1.0,
	-1.0, -1.0,
	 1.0,  1.0,
	-1.0,  1.0,
};
int gfx_cur_shader = -1;
int gfx_draw_call_tex_stack = 0;

struct GfxRT {
	GLuint fb;
	GLuint tex;
	int2 size;
};
GfxRT gfx_rt(float size) {
	GfxRT rt;

	rt.size = {(int)((float)gfx_size.w*size), (int)((float)gfx_size.h*size)};
	//GLuint frame_texture;
	glGenTextures(1, &rt.tex);
	glBindTexture(GL_TEXTURE_2D, rt.tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rt.size.w, rt.size.h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0); // todo: RGBA
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//GLuint framebuffer;
	glGenFramebuffers(1, &rt.fb);
	glBindFramebuffer(GL_FRAMEBUFFER, rt.fb);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rt.tex, 0);
	GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, draw_buffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		__debugbreak();
	}
	glViewport(0, 0, rt.size.w, rt.size.h);

	return rt;
}
void gfx_rtb(GfxRT rt) {
	glBindFramebuffer(GL_FRAMEBUFFER, rt.fb);
	glViewport(0, 0, rt.size.w, rt.size.h);
}

void gfx_sh(int prog_index) {
	GLint linked;
	glGetProgramiv(shader_progs[prog_index].prog, GL_LINK_STATUS, &linked);
	if (linked) {
		gfx_cur_shader = prog_index;
		glUseProgram(shader_progs[prog_index].prog);
	} else {
		debug_print("shader %i broken\n", prog_index);
	}
}
void gfx_sh(char *vs, char *fs) {
	int progi;
	for (int i = 0; i < shader_prog_count; ++i) {
		if (strcmp(shader_progs[i].vs_file, vs)==0 &&
			strcmp(shader_progs[i].fs_file, fs)==0) { // todo: string pool id compare
			progi = i;
			goto done;
		}
	}

	progi = create_shader_vf(vs, fs);

done:
	gfx_sh(progi);
}
void gfx_uf(char *name, float value) {
	int loc = glGetUniformLocation(shader_progs[gfx_cur_shader].prog, name); // todo: cache uniform names
	glUniform1f(loc, value);
}
void gfx_uf2(char *name, float a, float b) {
	int loc = glGetUniformLocation(shader_progs[gfx_cur_shader].prog, name); // todo: cache uniform names
	glUniform2f(loc, a, b);
}
void gfx_rtut(char *name, GfxRT rt) {
	glActiveTexture(GL_TEXTURE0 + gfx_draw_call_tex_stack);
	glBindTexture(GL_TEXTURE_2D, rt.tex);
	int u_tex = glGetUniformLocation(shader_progs[gfx_cur_shader].prog, name); // todo: cache
	glUniform1i(u_tex, 0);

	++gfx_draw_call_tex_stack;
}

void gfx_draw_call() {
	gfx_draw_call_tex_stack = 0;
}
void gfx_quad() {
	int u_pos = glGetAttribLocation(shader_progs[gfx_cur_shader].prog, "vertex_position"); // todo: cache attributes
	glEnableVertexAttribArray(u_pos);
	glVertexAttribPointer(u_pos, 2, GL_FLOAT, GL_FALSE, 0, gfx_quad_verts);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	gfx_draw_call();
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Rain rain = {};
	rain.window_width = 1920;
	rain.window_height = 1080;
	rain_init(&rain);
	SetWindowLongPtr(_win32.window, GWL_STYLE, WS_OVERLAPPED|WS_POPUP);
	SetWindowPos(_win32.window, NULL, 0, 0, rain.window_width, rain.window_height, SWP_SHOWWINDOW);
	load_opengl_extensions();

	int shader = create_shader_vf("shader.vert", "shader.frag");
	int blur_shader = create_shader_vf("shader.vert", "blur.frag");

	/*GLuint frame_texture;
	glGenTextures(1, &frame_texture);
	glBindTexture(GL_TEXTURE_2D, frame_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rain.window_width, rain.window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frame_texture, 0);
	GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, draw_buffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		__debugbreak();
	}
	glViewport(0, 0, rain.window_width, rain.window_height);*/

	GfxRT main_rt = gfx_rt(1.0f);
	GfxRT post_rt = gfx_rt(1.0f);
	GfxRT test_rt = gfx_rt(1.0f);

	//GLuint frame_texture2;
	//glGenTextures(1, &frame_texture2);
	//glBindTexture(GL_TEXTURE_2D, frame_texture2);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rain.window_width, rain.window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//GLuint framebuffer2;
	//glGenFramebuffers(1, &framebuffer2);
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frame_texture2, 0);
	////draw_buffers = {GL_COLOR_ATTACHMENT0};
	//glDrawBuffers(1, draw_buffers);
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
	//	__debugbreak();
	//}
	//glViewport(0, 0, rain.window_width, rain.window_height);

	while (!rain.quit) {
		rain_update(&rain);

		if (rain.keys[KEY_ESCAPE].released) {
			rain.quit = true;
		}

		update_shaders();

		/*glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, rain.window_width, rain.window_height);*/
		
		/*no_shader();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);*/

		float2 verts[] = {
			-1.0, -1.0,
			1.0, -1.0,
			1.0, 1.0,
			-1.0, -1.0,
			1.0, 1.0,
			-1.0, 1.0,
		};


		//use_shader(shader);
		/*int u_time = glGetUniformLocation(shader_progs[shader].prog, "time");
		glUniform1f(u_time, rain.time_s);*/
		/*int u_pos = glGetAttribLocation(shader_progs[shader].prog, "vertex_position");
		glEnableVertexAttribArray(u_pos);
		glVertexAttribPointer(u_pos, 2, GL_FLOAT, GL_FALSE, 0, verts);
		glDrawArrays(GL_TRIANGLES, 0, 6);*/


		// first pass
		gfx_rtb(main_rt);
		gfx_sh("shader.vert", "shader.frag");
		gfx_uf("time", rain.time_s);
		gfx_quad();

		// second pass
		gfx_rtb(post_rt);
		gfx_sh("shader.vert", "blur.frag");
		gfx_rtut("rt_tex", main_rt);
		gfx_uf2("resolution", rain.window_width, rain.window_height);
		gfx_quad();

		// third pass
		gfx_rtb(test_rt);
		gfx_sh("shader.vert", "test.frag");
		gfx_rtut("rt_tex", post_rt);
		gfx_quad();


		/*glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
		glViewport(0, 0, rain.window_width, rain.window_height);*/
		/*glBindTexture(GL_TEXTURE_2D, main_rt.tex);
		int u_tex = glGetUniformLocation(shader_progs[blur_shader].prog, "rt_tex");
		glUniform1i(u_tex, 0);*/
		/*int u_pos = glGetAttribLocation(shader_progs[blur_shader].prog, "vertex_position");
		//glUniform2f(glGetUniformLocation(shader_progs[blur_shader].prog, "resolution"), rain.window_width, rain.window_height);
		glEnableVertexAttribArray(u_pos);
		glVertexAttribPointer(u_pos, 2, GL_FLOAT, GL_FALSE, 0, verts);
		glDrawArrays(GL_TRIANGLES, 0, 6);*/

		// Blit to backbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, rain.window_width, rain.window_height);
		/*glBlitFramebuffer(0, 0, rain.window_width, rain.window_height,
						  0, 0, rain.window_width, rain.window_height,
						  GL_COLOR_BUFFER_BIT, GL_LINEAR);*/

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, test_rt.tex);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
		glEnd();
	}

	float a = 5.0;
	float b = a * 0.5;

	float c = 5.0f;
	float d = c * 0.5f;

	printf("%f %f \n", b, d);
}