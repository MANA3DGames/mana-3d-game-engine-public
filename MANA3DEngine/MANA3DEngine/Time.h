#ifndef TIME_H
#define TIME_H

namespace MANA3D
{
	class MANA3DEngine;


	class Time
	{
		friend MANA3DEngine;

	private:
		static void Init();
		static bool Update();


		static float deltaTime;
		static float startTime;
		static float lastTime;

		static int totalFrames;
		static int fpsCount;
		


	public:
		static float GetTime();
		static float GetDeltaTime();
		static int GetFPS();

		static float GetTimeNeededToDrawFrame();
	};
}

#endif // !TIME_H
