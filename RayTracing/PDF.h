#pragma once

#include "Vector3.h"
#include "ONB.h"
#include "Hittable.h"

class PDF
{
public:
	virtual ~PDF() {}

	virtual double Value(const Vector3 direction) const = 0;
	virtual Vector3 Generate()const = 0;
};

class CosinePDF : public PDF
{
public:
	CosinePDF(const Vector3 w);

	double Value(const Vector3 direction) const override;

	Vector3 Generate() const override;

private:
	ONB uvw;
};

class HittablePDF : public PDF
{
public:
    HittablePDF(const Hittable& _objects, const Position& _origin) : objects(_objects), origin(_origin){}

	double Value(const Vector3 direction) const override;

	Vector3 Generate() const override;

private:
    const Hittable& objects;
    Position origin;
};

class MixturePDF : public PDF
{
  public:
      MixturePDF(shared_ptr<PDF> pdf0, shared_ptr<PDF> pdf1);

      double Value(const Vector3 direction) const override;

	  Vector3 Generate() const override;

  private:
    shared_ptr<PDF> pdf[2];
};