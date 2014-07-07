#include "sphere.h"
#include "utils.h"



sphere::sphere():radius(1.0f),sqr_radius(1.0f)
{
	center.zeros();
}

sphere::sphere(float cx, float cy, float cz, float r):center(cx,cy,cz),radius(r)
{
	sqr_radius=r*r;
}

sphere::sphere(const tiny_vec<float,3>& c, float r):center(c),radius(r)
{
	sqr_radius=r*r;
}

void sphere::set_radius(float r)
{
	radius = r;
	sqr_radius = r*r;
}

void sphere::calc_normal(intersection_info* hit)const 
{
	tiny_vec<float,3> nml = hit->get_location()-center;
	nml.normalize();
	
	hit->set_normal(nml);
}

std::pair<tiny_vec<float,3>, tiny_vec<float,3> > sphere::calc_bounds()
{
	return std::make_pair(center-radius,center+radius);
}

bool sphere::closest_intersection(intersection_info* hit, float min_lambda, primitive* dont_hit)
{
	//student begin
	//see task 1.2.1
	if(dont_hit == this){
		return false;
	}
	tiny_vec<float,3> ray_from, ray_to, ray_center_vector;
	//ray_from = Quelle des Strahls
	//ray_to = Richtung des Strahls
	//ray_center_vector = Vektor von Quelle zum Mittelpunkt

	ray_from = hit->get_incoming_ray().get_origin();
	ray_to = hit->get_incoming_ray().get_direction();

	//center = Mittelpunkt der Kugel
	ray_center_vector = ray_from - center;

	float a, b, c, ts[2];
	//Quadratische Gleichung
	a = ray_to.sqr_length();
	b = 2*dot(ray_to,ray_center_vector);
	c = ray_center_vector.sqr_length() - sqr_radius;

	int lsg = solve_real_quadratic(a, b, c, ts);


	for(int i = 0; i < lsg; i++){
		//if(Schnittpunkt liegt im Intervall von lambda)
		if(min_lambda > ts[i] && ts[i] < hit->get_lambda()){
			hit->set_lambda(ts[i]);
			hit->set_object(this);
			calc_normal(hit);
			return true;
		}
	}

	return false;
	//student end
}

//(Strahl, vorderste Grenze, hinterste Grenze, dont_hit)
bool sphere::any_intersection(ray<float>& r,float min_lambda,float max_lambda, primitive* dont_hit)
{
	//student begin
	//see task 1.2.1
	if(dont_hit == this){
		return false;
	}

	tiny_vec<float,3> ray_from, ray_to, ray_center_vector;

	ray_from = r.get_origin();
	ray_to = r.get_direction();
	ray_center_vector = ray_from - center;

	float a, b, c, ts[2];
	//Quadratische Gleichung darstellen
	a = ray_to.sqr_length();
	b = 2*dot(ray_to,ray_center_vector);
	c = ray_center_vector.sqr_length() - sqr_radius;

	//lsg = Anzahl der Lösungen der quadr. Gl.
	int lsg = solve_real_quadratic(a, b, c, ts);
	
	switch(lsg){
		case 0:
			//keine Schnittpunkte
			return false;
		case 1:
			//if(Schnittpunkt liegt im Intervall von lamda)
			if(min_lambda <= ts[0] && ts[0] < max_lambda){
				return true;
			}
			break;
		case 2:
			//if(einer von beiden Schnittpunkten liegt im Intervall von lambda)
			if( (min_lambda <= ts[0] && ts[0] < max_lambda)  ||  (min_lambda <= ts[1] && ts[1] < max_lambda)){
				return true;
			}
			break;
	}

	return false;
	//student end

}
