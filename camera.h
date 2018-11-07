#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0*vec3(drand48(),drand48(),0) - vec3(1,1,0);
	} while (dot(p,p) >= 1.0);
	return p;
}

class camera {
public:
	camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, double vfov, double aspect, double aperture, double focus_dist) {
		lens_radius = aperture/2;
		double theta = vfov*M_PI/180;
		double half_height = tan(theta/2);
		double half_width = aspect * half_height;
 
		origin = lookFrom;
		w = unit_vector(lookFrom-lookAt);
		u = unit_vector(cross(vUp,w));
		v = cross(w,u);

		lower_left = origin - half_width *focus_dist*u - half_height*focus_dist*v - focus_dist*w;
		horizontal = 2*half_width*focus_dist*u;
		vertical = 2*half_height*focus_dist*v;
	}

	ray get_ray(double s, double t) { 
		vec3 rd = lens_radius*random_in_unit_disk();
		vec3 offset = rd.x() * u + rd.y() * v;
		return ray(origin+offset, lower_left+s*horizontal+t*vertical-origin-offset); 
	}

	vec3 origin;
	vec3 lower_left;
	vec3 horizontal;
	vec3 vertical;
	vec3 u,v,w;
	double lens_radius;
};

#endif