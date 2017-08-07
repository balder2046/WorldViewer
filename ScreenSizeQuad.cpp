
#include "ScreenSizeQuad.h"
#include <glm/glm.hpp>
using namespace glm;
CScreenSizeQuad::CScreenSizeQuad(void)
{
	
	//generate the cube object
	shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/fullscreen_quad_shader.vert");
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/fullscreen_quad_shader.frag");
	shader.CreateAndLinkProgram();
	shader.Use();
		shader.AddAttribute("vVertex");
		shader.AddUniform("textureMap"); 
		glUniform1i(shader("textureMap"), 0);
	shader.UnUse();

	glm::vec2* vertices=new glm::vec2[4];
	vertices[0]=glm::vec2(-1,-1);
	vertices[1]=glm::vec2( 1,-1);
	vertices[2]=glm::vec2( 1, 1);
	vertices[3]=glm::vec2(-1, 1);

	totalIndices = 2*3;
	GLushort* indices = new GLushort[totalIndices];

	int count = 0;

	//fill indices array
	GLushort* id=&indices[0];
	 
	*id++ = 0; 	*id++ = 1; 	*id++ = 2;
	*id++ = 0; 	*id++ = 2; 	*id++ = 3; 
	 
	//setup vao and vbo stuff
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVerticesID);
	glGenBuffers(1, &vboIndicesID);

	glBindVertexArray(vaoID);	

		glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
		glBufferData (GL_ARRAY_BUFFER, 8*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		 
		glEnableVertexAttribArray(shader["vVertex"]);
		glVertexAttribPointer(shader["vVertex"], 2, GL_FLOAT, GL_FALSE,0,0);
		  
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*totalIndices, &indices[0], GL_STATIC_DRAW);
		 
	glBindVertexArray(0);
	delete [] indices;
	delete [] vertices;
}

void CScreenSizeQuad::FillVertexBuffer(GLfloat* pBuffer) {
	return;
}

void CScreenSizeQuad::FillIndexBuffer(GLuint* pBuffer) {
	return;
}
CScreenSizeQuad::~CScreenSizeQuad(void)
{
}


void CScreenSizeQuad::Render(const float* MVP) {
	shader.Use();	
	glUniform1i(shader("textureMap"), 0);
	glBindVertexArray(vaoID);
	
	glDrawElements(GL_TRIANGLES, totalIndices, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	shader.UnUse();
}

GLuint g_QuadVAO = 0;
GLuint g_QuadVBO = 0;
GLuint g_QuadVIO = 0;
struct ScreenVertex_t
{
	vec2 pos;
	vec2 uv;
};
void InitFullScreenQuad() {
	glGenVertexArrays(1,&g_QuadVAO);
	glBindVertexArray(g_QuadVAO);
	// bind vertex buffer
	glGenBuffers(1,&g_QuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER,g_QuadVBO);
	ScreenVertex_t vertices[4];
	// for left down
	vertices[0].pos = vec2(-1,-1);
	vertices[0].uv = vec2(0,1);
	// right down
	vertices[1].pos =vec2( 1,-1);
	vertices[1].uv = vec2(1,1);
	// right top
	vertices[2].pos =vec2( 1, 1);
	vertices[2].uv = vec2(1,0);
	// left top
	vertices[3].pos=	vec2(-1, 1);
	vertices[3].uv = vec2(0,0);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	glGenBuffers(1,&g_QuadVIO);
	GLushort indices[6];

	int count = 0;

	//fill indices array
	GLushort* id = indices;

	*id++ = 0; 	*id++ = 1; 	*id++ = 2;
	*id++ = 0; 	*id++ = 2; 	*id++ = 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
	glBindVertexArray(0);

}
void DrawFullScreenQuad() {
    glBindVertexArray(g_QuadVAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,0);
}
