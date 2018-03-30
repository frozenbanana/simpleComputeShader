#version 450

in vec2 v_uvs;

out vec4 out_col;

layout (binding=0) uniform sampler2D gColour;
layout (binding=1) uniform sampler2D gComputed;

void main() {
  out_col = vec4( texture(gColour, v_uvs).rgb , 1.0f);

  if (v_uvs.x < 0.5f) {
    out_col = vec4( texture(gComputed, v_uvs).rgb , 1.0f);
  }
}
