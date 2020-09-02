#ifndef MRECTANGLE_H
#define MRECTANGLE_H

#include "MPrimitive.h"
using namespace MANA3D;

namespace MANA3D
{
	class MRectangle : public MPrimitive
	{
	protected:
		MRectangle();
		virtual ~MRectangle();

	public:
		static MRectangle* Create();

	};
}

#endif
