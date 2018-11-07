#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere: public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	vec3 center;
	double radius;
	material *mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = dot(oc, r.direction());
	double c = dot(oc, oc) - radius*radius;
	double d = b*b - a*c;
	if(d > 0) {
		double temp = (-b - sqrt(d))/a;
		if(temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(d))/a;
		if(temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

#endif