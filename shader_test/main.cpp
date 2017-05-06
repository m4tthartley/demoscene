
#include "w:/libs/rain.c"
#include <GL/glu.h>
#include "w:/libs/gfx.c"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	Rain rain = {};
	rain.window_width = 1920;
	rain.window_height = 1080;
	rain_init(&rain);
	SetWindowLongPtr(_win32.window, GWL_STYLE, WS_OVERLAPPED|WS_POPUP);
	SetWindowPos(_win32.window, NULL, 0, 0, rain.window_width, rain.window_height, SWP_SHOWWINDOW);
	load_opengl_extensions();

	int test_shader = create_shader_vf("test.vert", "test.frag");
	Shader shader = shader_from_file("shader.glsl", SHADER_VERTEX|SHADER_PIXEL);

	GLuint frame_texture;
	glGenTextures(1, &frame_texture);
	glBindTexture(GL_TEXTURE_2D, frame_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rain.window_width, rain.window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frame_texture, 0);
	GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, draw_buffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		__debugbreak();
	}
	glViewport(0, 0, rain.window_width, rain.window_height);

	while (!rain.quit) {
		rain_update(&rain);

		update_shaders();

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, rain.window_width, rain.window_height);

		no_shader();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		float2 verts[] = {
			-1.0, -1.0,
			1.0, -1.0,
			1.0, 1.0,
			-1.0, -1.0,
			1.0, 1.0,
			-1.0, 1.0,
		};

		use_shader(&shader);
		int u_time = glGetUniformLocation(shader.gl_program, "time");
		glUniform1f(u_time, rain.time_s);
		int u_pos = glGetAttribLocation(shader.gl_program, "vertex_position");
		glEnableVertexAttribArray(u_pos);
		glVertexAttribPointer(u_pos, 2, GL_FLOAT, GL_FALSE, 0, verts);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Blit to backbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, rain.window_width, rain.window_height);
		/*glBlitFramebuffer(0, 0, rain.window_width, rain.window_height,
						  0, 0, rain.window_width, rain.window_height,
						  GL_COLOR_BUFFER_BIT, GL_LINEAR);*/

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, frame_texture);
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