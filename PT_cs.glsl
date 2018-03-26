#version 450
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_image_load_store : enable

layout (rgba8, binding=0) uniform image2D texture_source0;						//Textures bound to 0 and 1 resp. that are used to
layout (rgba8, binding=1) uniform image2D texture_target0;						//acquire texture and save changes made to texture

layout (local_size_x=10 , local_size_y=10 , local_size_z=1) in;			//Local work-group size

void main() {
  ivec2 txlPos = ivec2(gl_GlobalInvocationID.xy);		//Get txl-pos

  vec4 result = imageLoad(texture_source0, txlPos);

  barrier();

  imageStore(texture_target0, txlPos, result);			//Save color in target texture
}
