#version 450

in vec2 v_uvs;

out vec4 out_col;

layout (binding=0) uniform sampler2D gColour;

void main() {
  out_col = vec4( texture(gColour, v_uvs).rgb , 1.0f);
}
