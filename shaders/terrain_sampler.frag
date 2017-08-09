#version 330 core
 
layout (location=0) out vec4 vFragColor;	//fragment shader output

//vertex shader input
smooth in vec3 worldpos;
smooth in vec2 uv;
//uniform
uniform mat4 matView;
uniform vec2 f;
uniform vec2 c;
uniform sampler2D textureMap;	//texture map
vec2 GetScreenPosFromWorldPos(vec3 worldpos)
{
    vec4 pos = vec4(worldpos,1);
	pos = matView * pos;
	float x = pos.x / pos.z;
	float y = pos.y / pos.z;
	float screenx = x * f.x + c.x;
	float screeny = y * f.y + c.y;
	
	return vec2(0.0,0.0);
}
void main()
{	
   	vec4 color = vec4(1.0);




	//return the average color
    vFragColor = vec4(color.r,color.g,color.b,1.0);
}