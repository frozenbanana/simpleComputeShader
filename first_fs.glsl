#version 440

in vec3 v_col;

out vec4 out_col;

void main() {
  out_col = vec4(v_col, 1.0);
}
