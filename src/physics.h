#pragma once
#include <iostream>

#include "vec3math.h"
#include "transform.h"

class body
{
    // mass in gr
    unsigned int mass = 100;
    
    vec3 last_position;
    vec3 acceleration = vec3(0,0,0);
    transform t = transform();
public:
    body()
    {
        last_position = t.position;
    }
    
    void set_mass(const unsigned int new_mass)
    {
        mass = new_mass;
    }
    double get_mass() const
    {
        return mass;
    }
    
    vec3 get_position() const
    {
        return t.position;
    }
    
    void update(u dt)
    {
        // integrate position
        const vec3 delta_position = t.position - last_position;
        last_position = t.position;
        
        // next position
        t.position = last_position + delta_position + acceleration/(u)mass;
        acceleration = vec3(0,0,-9810)*dt;

        std::cout << t.position.z << '\n';
    }
};