#include "Seeker.h"

Seeker::Seeker() :
    m_MAX_SPEED{10.0f},
    m_MIN_SPEED{1.0f},
    m_speed{0.0f}
{
    if (!m_texture.loadFromFile("ASSETS/alien.png"))
    {
        std::cout << "Image load failed" << std::endl;
    }
    m_shape.setTexture(m_texture);
    m_shape.setOrigin(m_shape.getScale().x / 2, m_shape.getScale().y / 2);

    srand(time(nullptr));
}

Seeker::~Seeker()
{
}

void Seeker::update(sf::RenderWindow const& t_window)
{
}

Steering Seeker::getSteering(MyVector3 t_target)
{
    return Steering();
}

float Seeker::getRotation(MyVector3 t_vector)
{
    return 0.0f;
}

float Seeker::radianToDegrees(float t_radian)
{
    return 0.0f;
}
