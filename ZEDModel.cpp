#include <GL/glew.h>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ZEDModel.hpp"
#include "glm/glm.hpp"
void CheckGLError();
Zed3D::Zed3D() {
    body_io.clear();
	path_mem.clear();
    vaoID = 0;
    vboID = 0;
    darktriID = 0;
    allumtriID = 0;

}

Zed3D::~Zed3D() {
	body_io.clear();
	path_mem.clear();
}

void Zed3D::setPath(sl::Transform &Path,std::vector<sl::Translation> path_history) {



}

void Zed3D::draw(glm::mat4 &pm) {
    shader.Use();
    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER,vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,darktriID);
    glUniform4f(shader("ObjColor"),DARK_COLOR.r,DARK_COLOR.g,DARK_COLOR.g,1.0f);
    glDrawElements(GL_TRIANGLES,NB_DARK_TRIANGLES * 3,GL_UNSIGNED_SHORT,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,allumtriID);
  //  const GLubyte *buf = gluErrorString(code);
    glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(pm));
    glUniform4f(shader("ObjColor"),ALLUMINIUM_COLOR.r,ALLUMINIUM_COLOR.g,ALLUMINIUM_COLOR.b,1.0f);
    glDrawElements(GL_TRIANGLES, NB_ALLUMINIUM_TRIANGLES * 3, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);


    shader.UnUse();
    return;





//    glPopMatrix();
}
struct Vertex_t
{
    float x;
    float y;
    float z;
    unsigned  int color;
};
void Zed3D::init() {
    sl::Transform path;
    GLuint tt;

    glGenVertexArrays(1,&vaoID);
    glBindVertexArray(vaoID);
    glGenBuffers(1,&vboID);
    glBindBuffer(GL_ARRAY_BUFFER,vboID);
    std::vector<Vertex_t> vertexs;
    int vertcount = sizeof(verticesZed) / sizeof(float) / 3;
    vertexs.resize(vertcount);
    for (int i = 0; i < vertcount; ++i)
    {
        vertexs[i].x = verticesZed[3 * i] * 100.0f;
        vertexs[i].y = verticesZed[3 * i + 1] * 100.0f;
        vertexs[i].z = verticesZed[3 * i + 2] * 100.0f;
    }
    unsigned int darkcolor,alluminiumcolor;
    darkcolor = glm::packUnorm4x8(glm::vec4(DARK_COLOR.r, DARK_COLOR.g, DARK_COLOR.b,1.0f));
    alluminiumcolor = glm::packUnorm4x8(glm::vec4(ALLUMINIUM_COLOR.r, ALLUMINIUM_COLOR.g, ALLUMINIUM_COLOR.b,1.0f));
    for (int i = 0; i < NB_ALLUMINIUM_TRIANGLES * 3; i += 3) {

        for (int j = 0; j < 3; j++) {
            double3color tmp;
            int index = alluminiumTriangles[i + j] - 1;
            vertexs[index].color = alluminiumcolor;

        }
    }

    for (int i = 0; i < NB_DARK_TRIANGLES * 3; i += 3) {
        for (int j = 0; j < 3; j++) {
            int index = darkTriangles[i + j] - 1;
            vertexs[index].color = darkcolor;
        }
    }
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex_t) * vertexs.size() ,&vertexs[0],GL_STATIC_DRAW);

    glGenBuffers(1,&darktriID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,darktriID);
    std::vector<short> indexs;
    indexs.resize(NB_DARK_TRIANGLES * 3);
    for (int i = 0; i < NB_DARK_TRIANGLES * 3; i++)
    {
        indexs[i] = (short)darkTriangles[i] - 1;

    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(short) * indexs.size(),&indexs[0],GL_STATIC_DRAW);
    glGenBuffers(1,&allumtriID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,allumtriID);

    indexs.resize(NB_ALLUMINIUM_TRIANGLES * 3);
    for (int i = 0; i < NB_ALLUMINIUM_TRIANGLES * 3; ++i)
    {
        indexs[i] = (short)alluminiumTriangles[i] - 1;
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(short) * indexs.size(), &indexs[0], GL_STATIC_DRAW);
    shader.LoadFromFile(GL_VERTEX_SHADER, "shaders/shader.vert");
    shader.LoadFromFile(GL_FRAGMENT_SHADER, "shaders/shader.frag");
    //compile and link shader
    shader.CreateAndLinkProgram();
    shader.Use();
    //add attributes and uniforms
    shader.AddAttribute("vVertex");
    
    shader.AddUniform("MVP");
    shader.AddUniform("ObjColor");
    shader.UnUse();
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex_t),(const void *)offsetof(Vertex_t,x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4,GL_BYTE,GL_TRUE,sizeof(Vertex_t),(const void *)offsetof(Vertex_t,color));
    glEnableVertexAttribArray(1);
	glBindVertexArray(0);
   // setPath(path, path_mem);
}
