#version 330 core
 
layout (location=0) out vec4 vFragColor;	//fragment shader output

//vertex shader input
smooth in vec2 uv;			//intepolated 2D texture coordinate
smooth in vec3 worldpos;

//uniform
uniform sampler2D textureMap;	//texture map
vec2 GetScreenPosFromWorldPos(vec3 worldpos)
{
    return vec2(0.0,0.0);
}
void main()
{	
   	vec4 color = vec4(0);

	color = texture(textureMap, uv);



	//return the average color
    vFragColor = vec4(color.r,color.g,color.b,1.0);
}