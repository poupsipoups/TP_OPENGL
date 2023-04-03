#pragma once

#include "glm/glm.hpp"

class TrackBallCamera {
private:
    float m_distance; // distance par rapport au centre de la scène
    float m_AngleX;   // angle effectué par la camera autour de X
    float m_AngleY;   // angle effectué par la camera autour de Y

public:
    TrackBallCamera()
        : m_distance(5.f), m_AngleX(0.f), m_AngleY(0.f){};

    void moveFront(float delta) { m_distance -= delta; };
    void rotateLeft(float degrees) { m_AngleY += degrees; };
    void rotateUp(float degrees) { m_AngleX += degrees; };

    glm::mat4 getViewMatrix() const;
};