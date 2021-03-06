//
// Created by ad on 17-7-26.
//

#include "Camera.h"
using namespace glm;
TrackBallCameraA::TrackBallCameraA(glm::vec3 p, glm::vec3 la) {
    position.x = p.x;
    position.y = p.y;
    position.z = p.z;

    lookAt.x = la.x;
    lookAt.y = la.y;
    lookAt.z = la.z;

    angleX = 0.0f;
    applyTransformations();
}

void TrackBallCameraA::applyTransformations() {
    forward = glm::vec3(lookAt.x - position.x,
                    lookAt.y - position.y,
                    lookAt.z - position.z);
    forward = glm::normalize(forward);
    if (fabs(forward.y) > 0.99f)
    {
        // up
        up = vec3(1.0f,0.0f,0.0f);
    } else
    {
        up = vec3(0.0f,1.0f,0.0f);
    }
    vec3 right = glm::cross(up,forward);
    left = -right;
    up = glm::cross(forward,right);
    up = glm::normalize(up);
    left = glm::normalize(left);


}

void TrackBallCameraA::show() {
    V = glm::lookAtRH(glm::tvec3<float>(position.x,position.y,position.z),glm::tvec3<float>(lookAt.x,lookAt.y,
                                                                                            lookAt.z),glm::vec3(up.x,up.y,up.z));
}

void TrackBallCameraA::rotation(float angle, vect3 v) {
    translate(vec3(-lookAt.x, -lookAt.y, -lookAt.z));
    vect3 temppos = vect3(position.x,position.y,position.z);
    temppos.rotate(angle, v);
    position = vec3(temppos.x,temppos.y,temppos.z);
    translate(vec3(lookAt.x, lookAt.y, lookAt.z));
    setAngleX();
}

void TrackBallCameraA::rotate(float speed, vect3 v) {
    float tmpA;
    float angle = speed / 360.0f;
    (v.x != 0.0f) ? tmpA = angleX - 90.0f + angle : tmpA = angleX - 90.0f;
    if (tmpA < 89.5f && tmpA > -89.5f) {
        translate(vec3(-lookAt.x, -lookAt.y, -lookAt.z));
        vect3 temppos = vect3(position.x,position.y,position.z);
        temppos.rotate(angle, v);
        position = vec3(temppos.x,temppos.y,temppos.z);
        translate(vec3(lookAt.x, lookAt.y, lookAt.z));
    }
    setAngleX();
}

void TrackBallCameraA::translate(vec3 v) {
    position.x += v.x;
    position.y += v.y;
    position.z += v.z;
}

void TrackBallCameraA::translateLookAt(vec3 v) {
    lookAt.x += v.x;
    lookAt.y += v.y;
    lookAt.z += v.z;
}

void TrackBallCameraA::translateAll(vec3 v) {
    translate(v);
    translateLookAt(v);
}

void TrackBallCameraA::zoom(float z) {
    float dist = vect3::length(vect3(position.x - lookAt.x, position.y - lookAt.y, position.z - lookAt.z));

    if (dist - z > z)
        translate(vec3(forward.x * z, forward.y * z, forward.z * z));
}

vec3 TrackBallCameraA::getPosition() {
    return vec3(position.x, position.y, position.z);
}

vec3 TrackBallCameraA::getPositionFromLookAt() {
    return vec3(position.x - lookAt.x, position.y - lookAt.y, position.z - lookAt.z);
}

vec3 TrackBallCameraA::getLookAt() {
    return vec3(lookAt.x, lookAt.y, lookAt.z);
}

vec3 TrackBallCameraA::getForward() {
    return vec3(forward.x, forward.y, forward.z);
}

vec3 TrackBallCameraA::getUp() {
    return vec3(up.x, up.y, up.z);
}

vec3 TrackBallCameraA::getLeft() {
    return vec3(left.x, left.y, left.z);
}

void TrackBallCameraA::setPosition(vec3 p) {
    position.x = p.x;
    position.y = p.y;
    position.z = p.z;
    setAngleX();
}

void TrackBallCameraA::setLookAt(vec3 p) {
    lookAt.x = p.x;
    lookAt.y = p.y;
    lookAt.z = p.z;
    setAngleX();
}

void TrackBallCameraA::setAngleX() {
    angleX = vect3::getAngle(vect3(position.x, position.y + 1, position.z),
                             vect3(position.x, position.y, position.z),
                             vect3(lookAt.x, lookAt.y, lookAt.z));
}

glm::vec2 TrackBallCameraA::GetViewportPosFromScreenPos(int x, int y) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    printf("the viewport is (%d,%d,%d,%d)\n",viewport[0],viewport[1],viewport[2],viewport[3]);
    // x is -1 ,1
    glm::vec2 pos;
    pos.x = -1.0f + ((float)x - viewport[0]) / viewport[2] * 2.0f;
    pos.y = 1.0f - ((float)y - viewport[0]) / (float)viewport[3] * 2.0f;
    return pos;
}

void TrackBallCameraA::GetRayFromScreenPos(int x, int y, glm::vec3 &vRayOrg, glm::vec3 &vRayDir) {
    vRayOrg = position;
    glm::vec2 viewportpos = GetViewportPosFromScreenPos(x,y);
    glm::vec4 vPos = vec4(viewportpos,0.0f,1.0f);
    vPos = vPos * 0.1f;// mul the znear
    mat4x4 matViewProjInv = glm::inverse(P * V);
    glm::vec4 vWorldPos = matViewProjInv * vPos;
    glm::vec3 vTarget = glm::vec3(vWorldPos.x,vWorldPos.y,vWorldPos.z);
    vRayDir = vTarget - position;
    printf("position is ()");
    float len = glm::length(vRayDir);
    printf("the length is %.2f\n",len);
    vRayDir = glm::normalize(vRayDir);
#ifdef GLM_DEPTH_ZERO_TO_ONE

#endif


}
