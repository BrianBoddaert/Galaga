#pragma once

#include "GameObject.h"

namespace Willem
{
	class Command
	{
	public:

		Command(int index) :m_ControllerIndex{ index } {};
		virtual ~Command() = default;
		Command(const Command&) = delete;
		Command& operator=(const Command&) = delete;
		Command(Command&&) = delete;
		Command& operator= (Command&&) = delete;

		virtual void Execute(const int& parameter = 0) = 0;
		virtual void Release(const int& parameter = 0) = 0;
		virtual void Undo() = 0;
		bool GetActivated() const { return m_Activated; };
		void SetActivated(bool b) { m_Activated = b; };
	private:
		bool m_Activated = false;
	protected:
		int m_ControllerIndex;
	};
}
