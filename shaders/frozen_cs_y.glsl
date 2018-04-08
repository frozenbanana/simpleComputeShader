#version 450
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_image_load_store : enable

layout (rgba8, binding=0) uniform image2D texture_source0;						//Textures bound to 0 and 1 resp. that are used to
layout (rgba8, binding=1) uniform image2D texture_target0;						//acquire texture and save changes made to texture

layout (local_size_x=1, local_size_y=400) in;			//Local work-group size

shared vec4 pixelStrip[400];

uniform float weight[5] = float[] (0.06136, 0.24477, 0.38774, 0.24477, 0.06136);		//Gaussian biggest in middle

void main() {
  ivec2 txlPos = ivec2(gl_GlobalInvocationID.xy);		//Get txl-pos

  // Store color result in pixelStrip
  // Index is the local position of workgroup in y-axis
  uint index = gl_LocalInvocationID.y;
  pixelStrip[index] = imageLoad(texture_source0, txlPos);
  barrier();

  vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);
  uint currentSample = 0;

  // Keep index in bounds of array during blur
  index = max(index, 2);
  index = min(index, 397);

  for(int i = -2; i < 3; i++) {
    currentSample = index + i;
    result += pixelStrip[currentSample] * weight[i+2];
  }

  imageStore(texture_target0, txlPos, result);			//Save color in target texture
}
