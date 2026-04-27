#include "config.h"
#include <OpenGL/gl.h>
#include <iostream>
#include <sys/types.h>

uint make_shader(const char *source, int type) {
  uint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  return shader;
}

uint make_program(uint vert_shader, uint frag_shader) {
  uint program = glCreateProgram();
  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);

  glLinkProgram(program);

  int success;
  char infoLog[512];
  glad_glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    glad_glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  return program;
}