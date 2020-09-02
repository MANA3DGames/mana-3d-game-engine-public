#include "MRectangle.h"
using namespace MANA3D;

MRectangle::MRectangle() : MPrimitive( "Rectangle" )
{
	AddMeshRenderer( rectangleMesh );
}

MRectangle::~MRectangle()
{

}


MRectangle* MRectangle::Create()
{
	MRectangle *primi = new MRectangle();
	//allPrimitives.push_back( primi );
	return primi;
}
