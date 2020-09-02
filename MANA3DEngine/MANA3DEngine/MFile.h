#ifndef MFILE_H
#define MFILE_H

#include "MString.h"

namespace MANA3D
{
	enum class MFileFilter
	{
		MFF_All,
		MFF_File,// = S_IFREG,
		MFF_Directory// = S_IFDIR
	};

	struct MFile
	{
		MString path;
		MString name;
		int type;
		bool isSelected = false;
	};
}

#endif