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

MixturePDF::MixturePDF(shared_ptr<PDF> pdf0, shared_ptr<PDF> pdf1)
{
    pdf[0] = pdf0;
    pdf[1] = pdf1;
}

double MixturePDF::Value(const Vector3 direction) const
{
    return 0.5 * pdf[0]->Value(direction) + 0.5 * pdf[1]->Value(direction);
}

Vector3 MixturePDF::Generate() const
{
    if (RandomDouble() < 0.5)
    {
        return pdf[0]->Generate();
    }
    else
    {
        return pdf[1]->Generate();
    }
}
