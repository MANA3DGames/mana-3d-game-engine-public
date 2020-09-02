#ifndef MTRIANGLE_H
#define MTRIANGLE_H

#include "MPrimitive.h"
using namespace MANA3D;

namespace MANA3D
{
	class MTriangle : public MPrimitive
	{
	protected:
			MTriangle();
		virtual ~MTriangle();

	public:
		static MTriangle* Create();
	};
}

#endif // !MTRIANGLE_H
