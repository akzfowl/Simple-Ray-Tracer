#include <iostream>
#include "sphere.h"
#include "hitablelist.h"
#include "float.h"
#include "material.h"
#include "camera.h"

hitable *random_scene() {
	int n = 500;
	hitable **list = new hitable*[n+1];
	list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5,0.5,0.5)));
	
	int k = 1;
	for(int i = -11;i<11;i++) {
		for(int j=-11;j<11;j++) {
			double choose_mat = drand48();
			vec3 center(i+0.9*drand48(), 0.2, j+0.9*drand48());
			if((center-vec3(4,0.2,0)).length() > 0.9) {
				if(choose_mat <0.8) {
					list[k++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
				}
				else if(choose_mat <0.95) {
					list[k++] = new sphere(center, 0.2, new metal(vec3(0.5*(1+drand48()),0.5*(1+drand48()),0.5*(1+drand48())), 0.5*drand48()));
				}
				else {
					list[k++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[k++] = new sphere(vec3(0,1,0), 1.0, new dielectric(1.5));
	list[k++] = new sphere(vec3(-4, 1,0), 1.0, new lambertian(vec3(0.4,0.2,0.1)));
	list[k++] = new sphere(vec3(4,1,0), 1.0, new metal(vec3(0.7,0.6,0.5),0.0));

	return new hitable_list(list,k);
}

vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;

	if(world->hit(r,0.001,DBL_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if(depth < 50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered)) {
			return attenuation*color(scattered,world,depth+1);
		}
		else{
			return vec3(0,0,0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		double t = 0.5*(unit_direction.y() + 1.0);
		return (1.0-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
	}
}

int main() {
	int nx = 1000;
	int ny = 500;
	int ns = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	vec3 lookFrom(13,2,3);
	vec3 lookAt(0,0,-1);
	double dist_to_focus = 10.0;//(lookFrom - lookAt).length();
	double aperture = 0.5;//2.0;

	hitable *list[5];
	list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1,0.2,0.5)));
	list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8,0.8,0.0)));
	list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8,0.6,0.2), 0.0));
	list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
	hitable *world = new hitable_list(list, 5);
	world = random_scene();

	camera cam(lookFrom, lookAt, vec3(0,1,0), 20, double(nx)/double(ny), aperture, dist_to_focus);

	for(int j=ny-1;j>=0;j--) {
		for(int i=0;i<nx;i++) {
			vec3 col(0,0,0);
			for(int s =0;s<ns;s++) {
				double u = double(i + drand48())/double(nx);
				double v = double(j + drand48())/double(ny);

				ray r = cam.get_ray(u,v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r,world,0);
			}
			
			col /= double(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			std::cout << ir << " " << ig << " " << ib <<  "\n";
		}
	}
}