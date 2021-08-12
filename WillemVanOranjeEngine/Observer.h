#pragma once
#include "GameObject.h"
namespace Willem
{
	class Observer
	{
	public:

		virtual ~Observer() = default;
		virtual void OnNotify(const Willem::GameObject* actor, EVENT event) = 0;
	};

} 