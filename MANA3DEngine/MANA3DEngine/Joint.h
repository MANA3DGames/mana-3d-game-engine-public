#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include "MString.h"
#include <assimp/matrix4x4.h>

using std::vector;


namespace MANA3D
{
	struct MVertexWeightPair
	{
		int vertexID;
		float weight;
	};

	class Joint
	{
	public:
		Joint( const int& index, const MString& name, const vector<MVertexWeightPair>& vertexWeightPairs, const aiMatrix4x4& transformMatrix, const aiMatrix4x4& offsetMatrix );
		~Joint();


		const int& GetIndex() const;
		const MString& GetName() const;
		const vector<MVertexWeightPair>& GetVertexWeightPairs();

		const aiMatrix4x4& GetTransformAIMatrix() const;
		const aiMatrix4x4& GetOffsetAIMatrix() const;

		const int GetChildrenCount() const;
		const vector<Joint*>& GetChildren() const;

		void AddJoint( Joint* child );


	private:
		int index;
		MString name;
		vector<MVertexWeightPair> vertexWeightPairs;

		aiMatrix4x4 transformMatrix;
		aiMatrix4x4 offsetMatrix;

		vector<Joint*> children;

	};
}

#endif
