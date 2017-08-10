#include "TestTri.h"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/vec4.hpp"
#include "RenderTextureFBO.h"
#include "GLSLShader.h"
#include "ScreenSizeQuad.h"
#include "Texture.h"
#include <opencv2/imgcodecs.hpp>
#include "Terrain.h"
#include "Camera.h"
#include "ZEDModel.hpp"
using namespace TRN;
using namespace glm;

#define GL_CHECK_ERRORS assert(glGetError()== GL_NO_ERROR);


using namespace std;
using namespace cv;

//screen size
const int WIDTH = 800;
const int HEIGHT = 600;
TrackBallCameraA camera(vec3(0.0f,20.0f,20.0f),vec3(0.0f,0.0f,0.0f));
//shader reference
GLSLShader shader;
Zed3D g_zed3D;

//vertex array and vertex buffer object IDs
GLuint vaoID;
GLuint vboVerticesID;
GLuint vboIndicesID;

//out vertex struct for interleaved attributes
struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
};

//triangle vertices and indices
Vertex vertices[3];
GLushort indices[3];

//projection and modelview matrices
glm::mat4  P = glm::mat4(1);
glm::mat4 MV = glm::mat4(1);
RenderTextureFBO *g_pRenderTextureFBO = 0;
CScreenSizeQuad *g_pScreenQuad = 0;
Texture g_Texture1;

GLuint g_testText;
Terrain g_Terrain;
int windowWidth = 0;
int windowHeight = 0;

bool Rotate = false;
int startx = 0;
int starty = 0;
bool Translate = false;
bool Zoom = false;
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            Rotate = true;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP)
            Rotate = false;
    }
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            Translate = true;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP)
            Translate = false;
    }

    if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {
            Zoom = true;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP)
            Zoom = false;
    }

    if ((button == 3) || (button == 4)) {
        if (state == GLUT_UP) return;
        if (button == 3)
            camera.zoom(0.5f);
        else
            camera.zoom(-0.5f);
    }
}

void motion(int x, int y) {
    if (Translate) {
        float Trans_x = (x - startx) / 30.0f;
        float Trans_y = (y - starty) / 30.0f;
        vec3 left = camera.getLeft();
        vec3 up = camera.getUp();
        camera.translateAll(vec3(left.x * Trans_x, left.y * Trans_x, left.z * Trans_x));
        camera.translateAll(vec3(up.x * -Trans_y, up.y * -Trans_y, up.z * -Trans_y));
        startx = x;
        starty = y;
    }

    if (Zoom) {
        camera.zoom((float) (y - starty) / 10.0f);
        starty = y;
    }

    if (Rotate) {
        float sensitivity = 100.0f;
        float Rot = y - starty;
        vec3 tmp = camera.getPositionFromLookAt();
        tmp.y = tmp.x;
        tmp.x = -tmp.z;
        tmp.z = tmp.y;
        tmp.y = 0.0f;
        tmp = normalize(tmp);
        camera.rotate(Rot * sensitivity, vect3(tmp.x,tmp.y,tmp.z));

        Rot = x - startx;
        camera.rotate(-Rot * sensitivity, vect3(0.0f, 1.0f, 0.0f));

        startx = x;
        starty = y;
    }
    glutPostRedisplay();
}

void keydown(unsigned char keycode,
	int x, int y)
{
	printf("the key is trigger %c at (%d,%d)\n", keycode,x,y);
	return;
}





void callback(GLenum source,
        GLenum type,
GLuint id,
        GLenum severity,
GLsizei length,
const GLchar *message,
const void *userParam)
{
    printf("info : %s\n",message);
    if (type == GL_DEBUG_TYPE_ERROR)
	{
		printf("error : %s\n",message);
		assert(false);
	}
}


//OpenGL initialization
GLSLShader g_shader;
void OnInit() {

    // Init for Debug
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	InitFullScreenQuad();
	vector<string> attrs,uniforms;
	attrs.push_back("vVertex");
	attrs.push_back("vUV");

    g_shader.Build("shaders/terrain_sampler",attrs,uniforms);
	glDebugMessageCallback(&callback, NULL);

    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	Mat mat;
	mat = imread("test.bmp", IMREAD_COLOR);
    glGenTextures(1,&g_testText);
    g_Terrain.SetPatchSize(32.0);
	g_Terrain.Init();
	GL_CHECK_ERRORS
    glBindTexture(GL_TEXTURE_2D,g_testText);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,256,256,0,GL_RGB,GL_UNSIGNED_BYTE,0);
	GL_CHECK_ERRORS
	g_pRenderTextureFBO = new RenderTextureFBO(256, 256);
    g_pRenderTextureFBO->Init();
	GL_CHECK_ERRORS
	g_pScreenQuad = new CScreenSizeQuad();
	GL_CHECK_ERRORS
		g_Texture1.FillWithMat(mat);
	g_Terrain.setTexture(g_Texture1.m_iTextureIndex);
		//load the shader
	//compile and link shader
    g_zed3D.init();
		cout << "Initialization successfull" << endl;
}

//release all allocated resources
void OnShutdown() {
	//Destroy shader

	//Destroy vao and vbo
	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);

	cout << "Shutdown successfull" << endl;
}

//resize event handler
void OnResize(int w, int h) {
	//set the viewport size

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    windowWidth = w;
    windowHeight = h;
	//setup the projection matrix
	P = glm::ortho(-1, 1, -1, 1);
}

//display callback function
void OnRender() {
    glEnable(GL_DEPTH_TEST);

    glClearColor(0, 0, 1, 1);
    glClearDepth(1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glActiveTexture(GL_TEXTURE0);
	glm::mat4 matView,matProj;
	matProj = glm::perspectiveRH(glm::radians(75.0f),windowWidth / (float)windowHeight,0.1f,1000.0f);

    camera.applyTransformations();
    camera.show();
	matView = camera.V;//glm::lookAt(vec3(0.0f,20.0f,0.0f),vec3(0.0f,0.0f,1.0f),vec3(0.0f,1.0f,0.0f));4
    glm::mat4 matProjView;
    matProjView = matProj * matView;
    g_zed3D.draw(matProjView);
	g_Terrain.Draw(matProj * matView);
	glutSwapBuffers();
	return;

}

int mainTestTri(int argc, char** argv) {
	//freeglut initialization calls
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("simple1");

    glutMouseFunc(mouse);
    glutMotionFunc(motion);
	glutKeyboardFunc(keydown);
	//glew initialization
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		cerr << "Error: " << glewGetErrorString(err) << endl;
	}
	else {
		if (GLEW_VERSION_3_3)
		{
			cout << "Driver supports OpenGL 3.3\nDetails:" << endl;
		}
	}
	err = glGetError(); //this is to ignore INVALID ENUM error 1282
	GL_CHECK_ERRORS

		//print information on screen
		cout << "\tUsing GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	GL_CHECK_ERRORS

		//opengl initialization
		OnInit();

	//callback hooks
	glutCloseFunc(OnShutdown);
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);

	//main loop call
	glutMainLoop();

	return 0;
}
