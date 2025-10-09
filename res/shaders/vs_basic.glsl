#shader vertex
#version 150 core

    in vec2 pos;
    in float sides;
    out float vSides;

    void main()
    {
        gl_Position = vec4(pos, 0.0, 1.0);
        vSides = sides;
    }