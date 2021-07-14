#pragma once
#include "GameObject.h"

namespace Willem
{
	class Observer
	{
	public:


		virtual ~Observer() = default;
		virtual void OnNotify(const GameObject* actor, Event event) = 0;
	};

} 