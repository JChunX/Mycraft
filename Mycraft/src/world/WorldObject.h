#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class WorldObject
{
public:
    glm::vec3 position;
    glm::quat orientation;

    WorldObject(glm::vec3 position, glm::quat orientation);
    WorldObject(glm::vec3 position=glm::vec3(0.0f, 0.0f, 0.0f), 
                glm::vec3 orientation=glm::vec3(0.0f, 0.0f, -1.0f));


    void Rotate(glm::vec3 rotation);
    void Rotate(glm::quat rotation);

    void LookAt(glm::vec3 target);

    void Translate(glm::vec3 translation);

    glm::vec3 GetEulerAngles();
    glm::vec3 GetHeadingVector();
};

#endif