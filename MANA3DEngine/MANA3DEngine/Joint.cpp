#include "Joint.h"
using namespace MANA3D;
using std::vector;

#include <glm\glm.hpp>
using namespace glm;


Joint::Joint( const int& index, const MString& name, const vector<MVertexWeightPair>& vertexWeightPairs, const aiMatrix4x4& transformMatrix, const aiMatrix4x4& offsetMatrix ) :
	index( index ), 
	name( name ), 
	vertexWeightPairs( vertexWeightPairs ), 
	transformMatrix( transformMatrix ), 
	offsetMatrix( offsetMatrix )
{
}

Joint::~Joint()
{
	while ( children.size() > 0 )
	{
		Joint* joint = *children.begin();
		children.erase( children.begin() );
		delete joint;
	}
    children.clear();
}


const int& Joint::GetIndex() const
{
	return index;
}

const MString& Joint::GetName() const 
{
	return name;
}

const vector<MVertexWeightPair>& Joint::GetVertexWeightPairs()
{
	return vertexWeightPairs;
}
		
const aiMatrix4x4& Joint::GetTransformAIMatrix() const
{
	return transformMatrix;
}
const aiMatrix4x4& Joint::GetOffsetAIMatrix() const
{
	return offsetMatrix;
}



const int Joint::GetChildrenCount() const
{
	return children.size();
}
const vector<Joint*>& Joint::GetChildren() const
{
	return children;
}


void Joint::AddJoint( Joint* child )
{
	children.push_back( child );
}