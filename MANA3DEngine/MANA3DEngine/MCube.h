#ifndef MCUBE_H
#define MCUBE_H

#include "MPrimitive.h"

namespace MANA3D
{
	class MCube : public MPrimitive
	{
	protected:
		MCube();
		virtual ~MCube();

	public:
		static MCube* Create();
	};
}

#endif