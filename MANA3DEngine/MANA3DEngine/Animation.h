#ifndef ANIMATION_H
#define ANIMATION_H

#include "MString.h"
#include <map>


namespace MANA3D
{
	class KeyFrame;

	class Animation
	{
	public:
		Animation( const MString& name, const float& ticksPerSecond, const float& duration, const int& channelsCount, const std::map<std::string, KeyFrame*>& jointKeyFramesMap );
		~Animation();


		const MString& GetName();

		const float& GetTicksPerSecond();
		const float& GetDuration();

		const int& GetChannelsCount();

		const KeyFrame* GetKeyFrame( const MString& jointName );

		const std::map<std::string, KeyFrame*>& GetJointKeyFramesMap();


	private:
		MString name;

		float ticksPerSecond;
		float duration;

		int channelsCount;

		std::map<std::string, KeyFrame*> jointKeyFramesMap;
	};
}

#endif
