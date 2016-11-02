#version 330

/*
 * Created by Florian Märkl <info@florianmaerkl.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define M_PI 3.1415926535897932384626433832795

uniform sampler2D tex_uni;
uniform vec4 min_max_uv_uni;
uniform float projection_angle_factor_uni;

in vec3 position_var;

out vec4 color_out;

void main(void)
{
	vec2 dir_h = position_var.xz;
	float length_h = length(dir_h);
	dir_h /= length_h;
	vec2 dir_v = normalize(vec2(length_h, position_var.y));

	vec2 sphere_coord = vec2(acos(dir_h.y), acos(dir_v.y)) * vec2(0.5 / M_PI, 1.0 / M_PI);
	if(dir_h.x > 0.0)
		sphere_coord.x = 1.0 - sphere_coord.x;

	sphere_coord.x -= 0.5;
	sphere_coord.x *= projection_angle_factor_uni;
	sphere_coord.x += 0.5;

	vec3 color;

	if(sphere_coord.x < 0.0 || sphere_coord.x > 1.0)
	{
		color = vec3(0.0);
	}
	else
	{
		vec2 uv = min_max_uv_uni.xy + (min_max_uv_uni.zw - min_max_uv_uni.xy) * sphere_coord;
		color = texture(tex_uni, uv).rgb;
	}

	color_out = vec4(color, 1.0);
}