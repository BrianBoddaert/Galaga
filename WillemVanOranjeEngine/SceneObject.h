#pragma once
#include <iostream>
#include <vector>
namespace Willem
{

	class SceneObject
	{
	public:
		virtual void Update(float deltaT) = 0;
		virtual void Render() const = 0;

		SceneObject(const std::string& name) :m_Name{ name } {};
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;

		virtual void AddTag(const std::string& tag) { m_Tags.push_back(tag); }
		virtual bool HasTag(const std::string& tag1) const
		{
			for (const std::string& tag2 : m_Tags)
			{
				if (tag1 == tag2)
					return true;
			}
			return false;
		}

		const std::string& GetName() const { return m_Name; };
	private:
		const std::string m_Name;
		std::vector<std::string> m_Tags;
	};
}
