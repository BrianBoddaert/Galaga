#pragma once
#include "Singleton.h"
#include "Structs.h"
#include <vector>
#include <string>
class TxtParser final : public Willem::Singleton<TxtParser>
{
public:

	void Parse(const std::string& path, std::vector < Willem::Vector2 > & positions);

private:
	friend class Willem::Singleton<TxtParser>;

};

