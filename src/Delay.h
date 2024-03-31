#pragma once

class Delay
{
public:

    float counter;
    float seconds;
    bool start;
    
    Delay(float seconds)
    {
        this->seconds = seconds;
        this->start = false;
        this->counter = 0.f;
    }

    bool Update(float deltaTime)
    {
        if (this->start)
        {
            this->counter += deltaTime;
            if (this->counter >= seconds)
            {
                counter -= seconds;
                this->start = false;
                return true;
            }
        }
        return false;
    }
};