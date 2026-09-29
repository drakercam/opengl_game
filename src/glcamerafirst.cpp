#include "glcamerafirst.h"

void camerafirst::update(mat4& view, gltime t) {
    this->front.x = cos(this->pitch) *  sin(this->yaw);
    this->front.y = sin(this->pitch);
    this->front.z = -cos(this->pitch) * cos(this->yaw);

    this->front = vec3::normalize(this->front);
    this->right = vec3::normalize(vec3::cross(this->front, this->worldUp));
    this->up = vec3::normalize(vec3::cross(this->right, this->front));

    mat4::identity(view);
    mat4::lookAt(view,
                 this->position,
                 this->position + this->front,
                 this->up
    );
}

void camerafirst::inputMouse(GLboolean constrainPitch) {

    float xOffset = getMouseDeltaX();
    float yOffset = -getMouseDeltaY();

    xOffset *= this->mouseSensitivity;
    yOffset *= this->mouseSensitivity;

    this->yaw += xOffset;
    this->pitch += yOffset;

    const float pitchLimit = ops::degreesToRadians(89.0f);

    if (constrainPitch) {

        if (this->pitch > pitchLimit) this->pitch = pitchLimit;
        if (this->pitch < -pitchLimit) this->pitch = -pitchLimit;
    }
}
