#pragma once

#include <chrono>

class Delay
{
public:
    float seconds;
    std::chrono::steady_clock::time_point startTime;
    bool start;

    Delay(float seconds) : seconds(seconds), start(false) {}

    void Start()
    {
        if (!start)
        {
            startTime = std::chrono::steady_clock::now();
            start = true;
        }
    }

    bool Update()
    {
        if (start)
        {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - startTime).count();

            if (elapsedTime >= seconds)
            {
                start = false;
                return true;
            }
        }
        return false;
    }

    float GetElapsedTime()
    {
        return std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - startTime).count();
    }
};