#include"WorldObject.h"

WorldObject::WorldObject(glm::vec3 position, glm::quat orientation)
    : position(position),
      orientation(orientation)
{

}

WorldObject::WorldObject(glm::vec3 position, glm::vec3 orientation)
    : WorldObject(position, glm::quat(orientation))
{

}

// Rotate the object using euler angles
void WorldObject::Rotate(glm::vec3 rotation)
{
    this->orientation = glm::quat(rotation) * this->orientation;
}

// Rotate the object using quaternions
void WorldObject::Rotate(glm::quat rotation)
{
    this->orientation = rotation * this->orientation;
}

// Look at a target
void WorldObject::LookAt(glm::vec3 target)
{
    // calculate quaternion from displacement vector
    glm::quat rotation = glm::quatLookAt(target - this->position, glm::vec3(0.0f, 1.0f, 0.0f));
}

// Translate the object in the world frame
void WorldObject::Translate(glm::vec3 translation)
{
    this->position += translation;
}

// Retrieve euler angles from quaternion
glm::vec3 WorldObject::GetEulerAngles()
{
    return glm::eulerAngles(this->orientation);
}

glm::vec3 WorldObject::GetHeadingVector()
{
    return glm::normalize(glm::inverse(this->orientation)*glm::vec3(0.0f, 0.0f, -1.0f));
}