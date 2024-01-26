#include "PDF.h"

double PDF::Value(const Vector3 direction) const
{
    return 1 / (4 * pi);
}

Vector3 PDF::Generate() const
{
    return RandomUnitVector();
}

CosinePDF::CosinePDF(const Vector3 w)
{
	uvw.BuildFromW(w);
}

double CosinePDF::Value(const Vector3 direction) const
{
	double cosineTheta = Dot(Unit(direction), uvw.W());
	return fmax(0, cosineTheta / pi);
}

Vector3 CosinePDF::Generate() const
{
	return uvw.Local(RandomCosineDirection());
}

double HittablePDF::Value(const Vector3 direction) const
{
    return objects.PDFValue(origin, direction);
}

Vector3 HittablePDF::Generate() const
{
    return objects.Random(origin);
}
