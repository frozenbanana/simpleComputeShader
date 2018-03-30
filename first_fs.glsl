#version 450

in vec3 v_col;

//out vec4 out_col;

layout(location = 0) out vec4 gColour;

void main() {
  //out_col = vec4(v_col, 1.0);
  gColour.rgb = v_col;
}
