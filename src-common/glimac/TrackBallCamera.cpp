#include "TrackballCamera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

glm::mat4 TrackBallCamera::getViewMatrix() const
{
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -m_distance));

    viewMatrix = glm::rotate(viewMatrix, glm::radians(m_AngleY), glm::vec3(1.f, 0, 0));

    viewMatrix = glm::rotate(viewMatrix, glm::radians(m_AngleX), glm::vec3(0, 1.f, 0));

    return viewMatrix;
}