#pragma once

#include <vector>

#include "MetaInfo.h"

#define STRINGIFY(x) #x

class IMetaInfoProvider
{
public:
	virtual ~IMetaInfoProvider() = default;
	virtual std::vector<MetaInfo> GetMetaInfo() = 0;
};