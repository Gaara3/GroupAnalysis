#pragma once
#include "ClusterPoint.h"
#include <vector>
using std::vector;
class Cluster
{
public:
	vector<ClusterPoint> points;
	Cluster();
	~Cluster();
};

