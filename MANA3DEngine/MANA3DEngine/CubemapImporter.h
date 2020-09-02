#ifndef CUBEMAP_IMPORTER_H
#define CUBEMAP_IMPORTER_H

#include "MString.h"

namespace MANA3D
{
	class MCubemap;
	struct MFile;

	class CubemapImporter
	{
		friend class MANA3DEngine;

	public:
		static MCubemap* Import( const MFile& file );
	};
}

#endif // !CUBEMAP_IMPORTER_H

