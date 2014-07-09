#include "phong.h"
#include "utils.h"
	
tiny_vec<float,3> phong::shade_diffuse(intersection_info* hit, light_source *light)
{
	tiny_vec<float,3> col(0.0f,0.0f,0.0f);
	tiny_vec<float,3> attenuation = light->get_attenuation_factors(hit); 
		
	//student begin
	//see task 1.3.2

	tiny_vec<float, 3> r_diff, w_in, normal, l_in;
	r_diff = get_diffuse();
	//light->calc_light_direction_and_distance(hit);
	normal = hit->get_normal();
	w_in = hit->get_direction_to_light();
	l_in = light->get_color();

	col = l_in * dot(w_in, normal);
	col.x() *= r_diff.x();
	col.y() *= r_diff.y();
	col.z() *= r_diff.z();

	col *= attenuation;

	//student end

	return col;	


}

tiny_vec<float,3> phong::shade_specular(intersection_info* hit, light_source *light)
{
	tiny_vec<float,3> col(0.0f,0.0f,0.0f);
	tiny_vec<float,3> attenuation = light->get_attenuation_factors(hit); 
	
	
	//student begin
	//see task 1.3.2

	tiny_vec<float, 3> r_spec, l_in, w_out, w_refl, normal, w_in;

	r_spec = get_specular();
	l_in = light->get_color();
	w_out = hit->get_direction_to_camera(),
	w_in = hit->get_direction_to_light();
	w_refl = 2 * dot(normal, w_in) * normal - w_in;
	normal = hit->get_normal();
	float m = get_shininess();

	if (dot(w_in, normal) <= 0){
		return col;
	}

	if (dot(w_out, w_refl) < 0){
		l_in *= pow(-dot(w_out, w_refl), m);
	}
	else{
		l_in *= pow(dot(w_out, w_refl), m);
	}
	
	col = l_in;

	col.x() *= r_spec.x();
	col.y() *= r_spec.y();
	col.z() *= r_spec.z();

	col *= attenuation;


	//student end

	return col;	


}