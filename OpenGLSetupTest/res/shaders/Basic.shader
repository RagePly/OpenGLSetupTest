#shader vertex
#version 330 core
layout(location = 0) in vec4 position; //layout(location = 0) references the attribute at index 0, which in our case is 2d position. then setting it to vec4 position. in stands for input
void main()
{
 gl_Position = position; //gl_position is a vec4, thats why we converted it before since we in the attribute assignment defined the attributes as having two components (a vec2)
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
void main()
{
 color = vec4(0.0,0.0,1.0,1.0);
};