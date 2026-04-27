#include "config.h"
#include "pipeline.cpp"
#include <iostream>
#include <string>

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(600, 400, "Flame Effect (Etornam)", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create a the window"
              << "\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to load OpenGL through GLAD"
              << "\n";
    return -1;
  }

  float vertices[] = {
      1.0f,  1.0f,  0.0f, // top right
      1.0f,  -1.0f, 0.0f, // bottom right
      -1.0f, -1.0f, 0.0f, // bottom left
      -1.0f, 1.0f,  0.0f  // top left
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  uint EBO;
	uint VAO;
	uint VB0;

  glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VB0);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VB0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // vert shader
  std::string vert_shader_src = R"(
		#version 330 core

		layout (location=0) in vec3 vertex_pos;

		void main() {
			gl_Position = vec4(vertex_pos, 1.0);
		}
	)";

  // frag shader
  std::string frag_shader_src = R"(
		#version 330 core
		uniform vec2 u_resolution;
		uniform float u_time;
		out vec4 screen_color;
		void main() {
			vec4 frag_color = vec4(0.0);
			float march_depth = 0.5;
			float noise_scale = 0.5;
			for (int march_iter = 0; march_iter < 70; march_iter++) {
				vec3 world_pos = vec3(march_depth * normalize(2.0 * vec3(gl_FragCoord.xy, 0.0) - vec3(u_resolution.x, u_resolution.y, u_resolution.x)));
				world_pos.z += 15.0 + cos(u_time);
				world_pos.xz *= mat2(
					(cos(u_time + world_pos.y / 4.0 + vec4(0.0, 8.0, 5.0, 0.0))).xy,
					(cos(u_time + world_pos.y / 4.0 + vec4(0.0, 8.0, 5.0, 0.0))).zw
				);
				noise_scale = 2.0;
				for (int noise_iter = 0; noise_iter < 6; noise_iter++) {
					noise_scale /= 0.8;
					world_pos += cos((world_pos.yzx - vec3(u_time, 0.0, 0.0) * 8.0) * noise_scale + u_time) / noise_scale;
				}
				float step_distance = 0.01 + abs(length(world_pos.xz) + world_pos.y * 0.3 - 1.0) / 9.0;
				march_depth += step_distance;
				frag_color += (sin(world_pos.y / 2.0 - vec4(0.0, 1.0, 2.0, 0.0)) + 1.1) / step_distance;
			}
			screen_color = tanh(frag_color / 1e3);
		}
	)";

  uint frag_shader = make_shader(frag_shader_src.c_str(), GL_FRAGMENT_SHADER);
  uint vert_shader = make_shader(vert_shader_src.c_str(), GL_VERTEX_SHADER);

  uint program = make_program(vert_shader, frag_shader);
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);

  glUseProgram(program);
  int time_loc = glGetUniformLocation(program, "u_time");
  int resolution_loc = glGetUniformLocation(program, "u_resolution");

  while (!glfwWindowShouldClose(window)) {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUniform1f(time_loc, static_cast<float>(glfwGetTime()));
    glUniform2f(resolution_loc, static_cast<float>(width), static_cast<float>(height));

    glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}