#include "triangle.h"
#include <limits>



triangle::triangle(const tiny_vec<float,3>& p0, const tiny_vec<float,3>& p1, const tiny_vec<float,3>& p2,
		const tiny_vec<float,3>& n1, const tiny_vec<float,3>& n2, const tiny_vec<float,3>& n3)
{
	v0=p0;
	edge1=p1-p0;
	edge2=p2-p0;
	nml1=n1;
	nml2=n2;
	nml3=n3;
	
	nml1.normalize();nml2.normalize();nml3.normalize();

	normal = cross(edge1,edge2);
	normal.normalize();
}
	
triangle::triangle(const tiny_vec<float,3>& p0, const tiny_vec<float,3>& p1, const tiny_vec<float,3>& p2)
	
{
	v0.set(p0(0),p0(1),p0(2));
	edge1=p1-p0;
	edge2=p2-p0;
	
	normal = cross(edge1,edge2);
	normal.normalize();
	nml1 = nml2 = nml3 = normal;
	
}



std::pair<tiny_vec<float,3>, tiny_vec<float,3> > triangle::calc_bounds()
{
	tiny_vec<float,3> minb(v0(0),v0(1),v0(2));
	tiny_vec<float,3> maxb(v0(0),v0(1),v0(2));

	for(int i = 0; i < 3; i++)
	{
		if(minb[i] > v0[i]+edge1[i])
			minb[i] = v0[i]+edge1[i];
		if(maxb[i] < v0[i]+edge1[i])
			maxb[i] = v0[i]+edge1[i];
		if(minb[i] > v0[i]+edge2[i])
			minb[i] = v0[i]+edge2[i];
		if(maxb[i] < v0[i]+edge2[i])
			maxb[i] = v0[i]+edge2[i];
	
	
	}
	return std::make_pair(minb,maxb);
}

void triangle::calc_normal(intersection_info* hit, float  u, float v)
{
	float w = 1-u-v;
	tiny_vec<float,3> nml = u*nml2+v*nml3+w*nml1;
	nml.normalize();
	hit->set_normal(nml);
	
}

bool triangle::closest_intersection(intersection_info* hit, float min_lambda, primitive* dont_hit)
{
	//student begin
	//see task 1.2.2
	if(dont_hit == this)
	{
		return false;
	}

	tiny_vec<float, 3> ray_from, ray_to, p0, p1, p2, x, normal1, normal2, normal3;
	float t, f, sigma0, sigma1, sigma2;

	// Definition der Geradenparamater
	ray_from = hit->get_incoming_ray().get_origin();
	ray_to = hit->get_incoming_ray().get_direction();

	// Definition der Dreieckspunkte
	p0 = v0;
	p1 = v0 + edge1;
	p2 = v0 + edge2;

	// Berechnung des parameters t
	t = dot(normal,p0-ray_from) / dot(normal,ray_to);

	// Schnittpunkt Gerade-Dreiecksebene
	x = ray_from + t * ray_to;

	// Berechnung der baryzentrischen Koordinaten
	f = 1 / normal.sqr_length();
	normal1 = cross(p1-x,p2-x);
	normal2 = cross(x-p0,p2-p0);
	normal3 = cross(p1-p0,x-p0);
	sigma0 = f * dot(normal,normal1); 
	sigma1 = f * dot(normal,normal2); 
	sigma2 = f * dot(normal,normal3); 

	
	// Pruefen ob Schnittpunkt im Dreieck (Baryzentrischen Koordinaten positiv)
	if(sigma0 >= 0 && sigma1 >= 0 && sigma2 >= 0)
	{
		if (t > 0 && t > min_lambda && t < hit->get_lambda())
			{
				hit->set_lambda(t);
				hit->set_object(this);
				calc_normal(hit, sigma0, sigma1);
				return true;
			}
	}
	
	return false;
	//student end
}

bool triangle::any_intersection(ray<float>& r,float min_lambda, float max_lambda,primitive* dont_hit)
{
	
	//student begin
	//see task 1.2.2
	if(dont_hit == this){
		return false;
	}

	tiny_vec<float, 3> ray_from, ray_to, p0, p1, p2, x, normal1, normal2, normal3;
	float t, f, sigma0, sigma1, sigma2;

	// Definition der Geradenparamater
	ray_from = r.get_origin();
	ray_to = r.get_direction();

	// Definition der Eckpunkte
	p0 = v0;
	p1 = v0 + edge1;
	p2 = v0 + edge2;

	// Berechnung des Parameters t
	t = dot(normal, p0-ray_from) / dot(normal, ray_to);
	
	// Berechnung des Schnittpunkts Gerade-Dreiecksebene
	x = ray_from + t * ray_to;

	// Berechnung der Baryzentrischen Koordinaten
	f = 1 / normal.sqr_length();
	normal1 = cross(p1-x, p2-x);
	normal2 = cross(x-p0, p2-p0);
	normal3 = cross(p1-p0, x-p0);
	sigma0 = f * dot(normal, normal1); 
	sigma1 = f * dot(normal, normal2); 
	sigma2 = f * dot(normal, normal3); 

	// Pruefen ob Schnittpunkt im Dreieck liegt (Baryzentrische Koordinaten positiv)
	if(sigma0 >= 0 && sigma1 >= 0 && sigma2 >= 0){
		if (t > 0 && t > min_lambda && t < max_lambda)
		{
			return true;
		}
	}
	return false;
	//student end
	

}
