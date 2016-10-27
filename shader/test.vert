#version 330

in vec3 vertex_attr;
in vec3 color_attr;

out vec3 color_var;

void main(void)
{
	color_var = color_attr;
	gl_Position = vec4(vertex_attr, 1.0);
}