#ifndef MANA3D_TRANSFORM_H
#define MANA3D_TRANSFORM_H

#include "Component.h"
#include "Debug.h"
#include "Math3D.h"
#include "MString.h"

namespace MANA3D
{
	class Transform : public Component
	{
		friend class GameObject;
		friend class SceneManager;

	protected:
		Transform();
		virtual ~Transform();

		void Update() override;

		void CopyFrom( Component* ) override;

	public:

		void SetPosition( const glm::vec3& pos );
		void SetScale( const glm::vec3& scale );
		void SetRotation( const glm::quat& rotation );
		void SetEulerAngles( const glm::vec3& euler );
		void SetEulerAngles( const float& x, const float& y, const float& z );

		void SetLocalPosition( const glm::vec3& pos );
		void SetLocalScale( const glm::vec3& scale );
		void SetLocalRotation( const glm::quat& rotation );
		void SetLocalEulerAngles( const glm::vec3& euler );
		void SetLocalEulerAngles( const float& x, const float& y, const float& z );

		void Translate( const glm::vec3& val );
		void TranslateX( const float& val );
		void TranslateY( const float& val );
		void TranslateZ( const float& val );

		void TranslateLocally( const glm::vec3& val );
		void TranslateXLocally( const float& val );
		void TranslateYLocally( const float& val );
		void TranslateZLocally( const float& val );

		void Rotate( const float& x, const float& y, const float& z );
		void Rotate( const glm::vec3& euler );
		void RotateX( const float& degree );
		void RotateY( const float& degree );
		void RotateZ( const float& degree );
		
		void RotateLocally( const float& x, const float& y, const float& z );
		void RotateLocally( const glm::vec3& euler );
		void RotateXLocally( const float& degree );
		void RotateYLocally( const float& degree );
		void RotateZLocally( const float& degree );

		void Scale( const glm::vec3& scale );
		void ScaleX( const float& val );
		void ScaleY( const float& val );
		void ScaleZ( const float& val );

		void ScaleLocally( const glm::vec3& scale );
		void ScaleXLocally( const float& val );
		void ScaleYLocally( const float& val );
		void ScaleZLocally( const float& val );

		void PrintModelMatrix();


		glm::vec3 GetPosition();
		glm::quat GetRotation();
		glm::vec3 GetEulerAngles();
		glm::vec3 GetScale();

		glm::vec3 GetLocalPosition();
		glm::quat GetLocalRotation();
		glm::vec3 GetLocalEulerAngles();
		glm::vec3 GetLocalScale();


		void SetRight( const glm::vec3& right );
		void SetForward( const glm::vec3& forward );
		void SetUp( const glm::vec3& up );

		glm::vec3 GetRight();
		glm::vec3 GetForward();
		glm::vec3 GetUp();


		void SetLocalModelMatrix( const glm::mat4& modelMat4 );
		glm::mat4 GetLocalModelMatrix();
		glm::mat4 GetModelMatrix();

		void DecomposeModelMatrix( glm::vec3& scale, glm::quat& rotation, glm::vec3& translation, glm::vec3& skew, glm::vec4& perspective );



		void SetParent( Transform* );
		Transform* GetParent();

		// Finds a child at given index.
		// * index: given index.
		Transform* GetChild( const unsigned int& index );
		Transform* FindChild( const MString& name );

		// Return a list of chidren GameObjects of this GameObject.
		std::list<Transform*>& GetChildren();
		size_t GetChildCount();
		void DetachChildren();


	private:
		// Parent GameObject of this GameObject. 
		Transform* parent;								

		// List of chidren GameObjects of this GameObject.	
		std::list<Transform*> children;

		glm::mat4 modelMatrix;
	};
}

#endif