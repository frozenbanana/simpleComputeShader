#version 440
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_image_load_store : enable

layout (rgba8, binding=0) uniform image2D texture_source0;						//Textures bound to 0 and 1 resp. that are used to
layout (rgba8, binding=1) uniform image2D texture_target0;						//acquire texture and save changes made to texture

layout (local_size_x=10 , local_size_y=10 , local_size_z=1) in;			//Local work-group size

//shared vec4 row[640];
//shared vec4 col[480];
shared vec4 both[2][640];

uniform ivec2 xORy;														//A variable deciding if blur is made vertically or horizontally

//uniform float weight[5] = float[] (0.38774, 0.06136, 0.06136, 0.24477, 0.24477);		//Gaussian curve numbers [Declining]
uniform float weight[5] = float[] (0.24477, 0.06136, 0.38774, 0.06136, 0.24477);		//Gaussian curve numbers [Spread]

void main() {
  ivec2 txlPos;		//A variable keeping track of where on the texture current texel is from
  vec4 result = vec4(0.0, 0.0, 0.0, 0.0);		//A variable to store color
  //vec2 offset;		//A vector to tell where new values should be gotten from
  //ivec2 plus_offset;
  //ivec2 minus_offset;
  //float weight[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f};

  txlPos = ivec2(gl_GlobalInvocationID.xy);		//Get txl-pos

  //row[txlPos.x] = imageLoad(texture_source0, txlPos);
  //col[txlPos.y] = row[txlPos.x];
  both[0][txlPos.x] = imageLoad(texture_source0, txlPos);
  both[1][txlPos.y] = both[0][txlPos.x];

  barrier();

/*
  result = imageLoad(texture_source0, txlPos);			//Get color value

  for(int i = 1; i < 5; i++){
  	offset = ivec2(txlPos.x * i, txlPos.y * i);

  	offset = dot(offset, xORy) * xORy;
  	//Instead of sending in an boolean, send in an vec2 with one 1 and one 0.
  	//Dot multiply the position by that vector to eliminate the term not needed
  	//Scalar multiply the dot product with the axis vector to
  	//get a vector with the offset in either x or y and 0 in the other

  	plus_offset = txlPos + ivec2(offset);
  	minus_offset = txlPos - ivec2(offset);

    result += imageLoad(texture_source0, plus_offset) * weight[i];		//NTS: Fetching a value outside the perimeter of the texture
  	result += imageLoad(texture_source0, minus_offset) * weight[i];		//will return only zeroes, additivity will cancel out any fault
  }
*/

  int maxX_maxY[2] = {640, 480};
  int curX_curY[2] = {txlPos.x, txlPos.y};

  for (int i = -2; i < 3; i++) {
    int mx = maxX_maxY[ xORy.y ];   //Get the max value for the pass
    int cr = curX_curY[ xORy.y ];   //Get the current value for the pass

    int index = min( max(cr + i , 0) , mx );  //Determine if the index will be out-of-bounds

    result += both[xORy.y][index] * weight[i+2];
  }

  //result = vec4(vec2(gl_GlobalInvocationID.xy) / vec2(640,480), 0.0, 1.0);

  imageStore(texture_target0, txlPos, result);			//Save color in target texture
}

