#pragma once

#include <core/material/material.h>

class CORE_API MaterialDictionary : public std::unordered_map<std::string, Material>
{
public:
	MaterialDictionary();
	virtual ~MaterialDictionary();
};