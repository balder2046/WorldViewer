//
// Created by ad on 17-7-26.
//

#ifndef WORLDVIEWER_CAMERA_H
#define WORLDVIEWER_CAMERA_H




#include "utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera {

};
class TrackBallCameraA {
public:

    TrackBallCameraA() {
    };
    TrackBallCameraA(glm::vec3 p, glm::vec3 la);
    void applyTransformations();
    void show();
    void rotation(float angle, vect3 v);
    void rotate(float speed, vect3 v);
    void translate(glm::vec3 v);
    void translateLookAt(glm::vec3 v);
    void translateAll(glm::vec3 v);
    void zoom(float z);

    glm::vec3 getPosition();
    glm::vec3 getPositionFromLookAt();
    glm::vec3 getLookAt();
    glm::vec3 getForward();
    glm::vec3 getUp();
    glm::vec3 getLeft();

    void setPosition(glm::vec3 p);
    void setLookAt(glm::vec3 p);
    glm::vec2 GetViewportPosFromScreenPos(int x,int y);
    void GetRayFromScreenPos(int x,int y,glm::vec3 &vRayOrg,glm::vec3 &vRayDir);
private:
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 left;
    float angleX;

    void setAngleX();
public:
    glm::mat4 P;
    glm::mat4 V;
};

#endif //WORLDVIEWER_CAMERA_H
