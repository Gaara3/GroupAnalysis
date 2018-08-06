#pragma once
#include "OriginPoint.h"
class ClusterPoint :
	public OriginPoint
{
public:
	int id;
	ClusterPoint(int id, OriginPoint op);
	ClusterPoint();
	~ClusterPoint();
};

