#pragma once

class TrackBallCamera {
private:
    float m_distance; // distance par rapport au centre de la scène
    float m_AngleX;   // angle effectué par la camera autour de X
    float m_AngleY;   // angle effectué par la camera autour de Y

    TrackBallCamera()
        : m_distance(5.f), m_AngleX(0.f), m_AngleY(0.f){};
};