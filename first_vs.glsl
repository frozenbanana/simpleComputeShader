#version 440

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_col;

out vec3 v_col;

void main() {
  gl_Position = vec4(in_pos, 1.0);

  v_col = in_col;
}
