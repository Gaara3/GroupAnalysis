#include "Cluster.h"



Cluster::Cluster(vector<ClusterPoint> points)
{
	this->points.assign(points.begin(), points.end());
}

Cluster::Cluster()
{
}


Cluster::~Cluster()
{
}
