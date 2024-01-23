#include "Sphere.h"

bool Sphere::Hit(const Ray& ray, Interval rayTime, HitInfo& hitInfo) const
{
	Position center1 = isMoving ? Center(ray.time()) : mCenter;

	Vector3 oC = ray.GetOrigin() - mCenter;
	double a = ray.GetDirection().SquaredLength();
	double halfB = Dot(oC, ray.GetDirection());
	double c = oC.SquaredLength() - mRadius * mRadius;

	double discriminant = halfB * halfB - a * c;
	if (discriminant < 0) return false;
	double sqrtDiscriminant = sqrt(discriminant);

	// Find the nearest root withing the min max time frame
	double root = (-halfB - sqrtDiscriminant) / a;
	if (!rayTime.Surrounds(root))
	{
		root = (-halfB + sqrtDiscriminant) / a;
		if (!rayTime.Surrounds(root))
			return false;
	}

	hitInfo.time = root;
	hitInfo.coordinates = ray.At(root);
	Vector3 outwardNormal = (hitInfo.coordinates - mCenter) / mRadius;
	hitInfo.SetFaceNormal(ray, outwardNormal);

	GetSphereUV(outwardNormal, hitInfo.x, hitInfo.y);

	hitInfo.mat = mat;

	return true;
}

Position Sphere::Center(double time) const
{
	// Linearly interpolate from center1 to center2 according to time, where t=0 yields
	// center1, and t=1 yields center2.
	return mCenter + time * center_vec;
}

void Sphere::GetSphereUV(const Position& position, double& U, double& V)
{
	// position : a given point on the sphere of radius one, centered at the origin
	// U : returned value [0,1] of angle around the Y axis from X =- 1
	// V : returned value [0,1] of angle from Y =- 1 to Y =+ 1
	// 
	//	<1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
	//	<0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
	//	<0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

	double theta = acos(-position.y);
	double phi = atan2(-position.z, position.x) + pi;

	U = phi / (2 * pi);
	V = theta / pi;
}
