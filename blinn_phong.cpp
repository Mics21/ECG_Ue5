#include "blinn_phong.h"
#include "utils.h"



tiny_vec<float,3> blinn_phong::shade_specular(intersection_info* hit, light_source *light)
{
		
	tiny_vec<float,3> attenuation = light->get_attenuation_factors(hit); 
	tiny_vec<float,3> col(0.0f,0.0f,0.0f);
	//studen begin
	//see task 1.3.1

	tiny_vec<float, 3> r_spec, l_in, w_out, w_refl, normal, w_in, w_half;

	r_spec = get_specular();
	l_in = light->get_color();
	w_out = hit->get_direction_to_camera(),
	w_in = hit->get_direction_to_light();
	w_refl = 2 * dot(normal, w_in) * normal - w_in;
	w_half = (w_in + w_out) / ((w_in + w_out).length());
	normal = hit->get_normal();
	float m = get_shininess();

	if (dot(w_in, normal) <= 0){
		return col;
	}

	if (dot(w_out, w_refl) < 0){
		l_in *= pow(-dot(w_half, normal), m);
	}
	else{
		l_in *= pow(dot(w_half, normal), m);
	}

	col = l_in;

	col.x() *= r_spec.x();
	col.y() *= r_spec.y();
	col.z() *= r_spec.z();

	col *= attenuation;
	
	//student end
	return col;	

}

