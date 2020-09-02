#ifndef MANA3D_FRAMEWORK_H
#define MANA3D_FRAMEWORK_H

namespace MANA3D
{
	class MANA3DFramework
	{
		friend class MANA3DEngine;

	private:
		MANA3DFramework(){}
		~MANA3DFramework(){}

	private:
		void OnStart();
		void OnUpdate();
		void OnTerminate();
	};
}

#endif // !MANA3DFRAMEWORK_H
