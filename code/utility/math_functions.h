#pragma once

#include <algorithm>
#include <math.h>
#include <SFML/Graphics.hpp>

using namespace sf;

typedef sf::Vector2f (*TweenFunction)(sf::Vector2f from, sf::Vector2f to, float t);

inline float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

inline float smoothstep(float from, float to, float t)
{
    t = std::clamp(t, 0.f, 1.f);
    t = (float)(-2.0 * (double)t * (double)t * (double)t + 3 * (double)t * (double)t);
    return (float)((double)to * (double)t + (double)from * (1.0 - (double)t));
}


inline Vector2f smoothstep(Vector2f from, Vector2f to, float t)
{
    return {
        smoothstep(from.x, to.x, t),
        smoothstep(from.y, to.y, t),
    };
}

inline float easeOutQuint(float from, float to, float t)
{
    t = 1 - std::pow(1 - t, 5);
    return lerp(from, to, t);
}

inline Vector2f easeOutQuint(Vector2f from, Vector2f to, float t)
{
    return {
        easeOutQuint(from.x, to.x, t),
        easeOutQuint(from.y, to.y, t),
    };
}

inline float smoothDamp(float current, float target, float &currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
{
    smoothTime = std::max(0.0001f, smoothTime);
    float num = 2.0f / smoothTime;
    float num2 = num * deltaTime;
    float num3 = 1.f / (1.f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
    float num4 = current - target;
    float num5 = target;
    float num6 = maxSpeed * smoothTime;
    num4 = std::clamp(num4, -num6, num6);
    target = current - num4;
    float num7 = (currentVelocity + num * num4) * deltaTime;
    currentVelocity = (currentVelocity - num * num7) * num3;
    float num8 = target + (num4 + num7) * num3;
    if (num5 - current > 0.f == num8 > num5)
    {
        num8 = num5;
        currentVelocity = (num8 - num5) / deltaTime;
    }
    return num8;
}

inline Vector2f smoothDamp(Vector2f current, Vector2f target, Vector2f &currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
{
    return {
        smoothDamp(current.x, target.x, currentVelocity.x, smoothTime, maxSpeed, deltaTime),
        smoothDamp(current.y, target.y, currentVelocity.y, smoothTime, maxSpeed, deltaTime),
    };
}