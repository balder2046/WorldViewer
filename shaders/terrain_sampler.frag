#version 330 core

layout (location=0) out vec4 vFragColor;	//fragment shader output

//vertex shader input
smooth in vec3 worldpos;
//uniform
uniform mat4 viewProj;
uniform sampler2D textureMap;	//texture map
vec2 GetScreenPosFromWorldPos(vec3 worldpos)
{
    vec4 pos = vec4(worldpos,1);
	pos = viewProj * pos;
	float x = (pos.x / pos.z + 1) * 0.5;
	float y = (pos.y / pos.z + 1) * 0.5;
    // x,y in range[0,1]
	return vec2(x,y);
}
void main()
{	
	vec2 uv = GetScreenPosFromWorldPos(worldpos);
	vec4 sampColor = texture(textureMap,uv);
    vFragColor = vec4(sampColor.r,sampColor.g,sampColor.b,1.0);
	
}