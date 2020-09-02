#include "Animation.h"
#include "KeyFrame.h"
using namespace MANA3D;
using std::map;
using std::string;


Animation::Animation( const MString& name, const float& ticksPerSecond, const float& duration, const int& channelsCount, const map<string, KeyFrame*>& jointKeyFramesMap ) :
	name( name ),
	ticksPerSecond( ticksPerSecond ),
	duration( duration ),
	channelsCount( channelsCount ),
	jointKeyFramesMap( jointKeyFramesMap )
{
}

Animation::~Animation()
{
	for ( map<string, KeyFrame*>::iterator it = jointKeyFramesMap.begin(); it != jointKeyFramesMap.end(); ++it )
	{
		delete it->second;
	}
	jointKeyFramesMap.clear();
}



const MString& Animation::GetName()
{
	return name;
}

const float& Animation::GetTicksPerSecond()
{
	return ticksPerSecond;
}

const float& Animation::GetDuration()
{
	return duration;
}

const int& Animation::GetChannelsCount()
{
	return channelsCount;
}


const KeyFrame* Animation::GetKeyFrame( const MString& jointName )
{
	map<string, KeyFrame*>::iterator it = jointKeyFramesMap.find( jointName.GetStr() );
	if ( it != jointKeyFramesMap.end() )
		return it->second;

	return nullptr;
}

const map<string, KeyFrame*>& Animation::GetJointKeyFramesMap()
{
	return jointKeyFramesMap;
}