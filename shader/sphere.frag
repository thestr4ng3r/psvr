#version 330

#define M_PI 3.1415926535897932384626433832795

uniform sampler2D tex_uni;
uniform vec4 min_max_uv_uni;

in vec3 uv_var;
in vec3 position_var;

out vec4 color_out;

void main(void)
{
	vec2 dir_h = position_var.xz;
	float length_h = length(dir_h);
	dir_h /= length_h;
	vec2 dir_v = normalize(vec2(length_h, position_var.y));

	vec2 sphere_coord = vec2(asin(dir_h.y), acos(dir_v.y)) * (2.0 / M_PI, 1.0 / M_PI);
	if(dir_h.x < 0.0)
		sphere_coord.x = 1.0 - sphere_coord.x;

	vec2 uv = min_max_uv_uni.xy + (min_max_uv_uni.zw - min_max_uv_uni.xy) * sphere_coord;

	vec3 color = texture(tex_uni, uv).rgb;
	color_out = vec4(color, 1.0);
}