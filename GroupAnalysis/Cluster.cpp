#include "Cluster.h"
#include "MiningTool.h"


Cluster::Cluster(vector<ClusterPoint> points)
{
	subGraph = nullptr;
	this->points.assign(points.begin(), points.end());
	this->clusterSize = points.size();
}

Cluster::Cluster(vector<ClusterPoint> points, double **adjMat)
{
	this->points.assign(points.begin(), points.end());
	this->clusterSize = points.size();
	setSubGraph(adjMat);
}

void Cluster::resetSubGraph()
{
	for (int i = 0; i < clusterSize; ++i)
		delete[] subGraph[i];
	delete[] subGraph;
}

void Cluster::setSubGraph(double ** adjMat)
{
	//this->clusterSize = (int)points.size();
	this->subGraph = new double*[clusterSize];
	for (int i = 0; i<clusterSize; ++i)
		this->subGraph[i] = new double[clusterSize];
	for (int i = 0; i < clusterSize; ++i) {
		for (int j = 0; j <= i; ++j) {
			if (i == j)
				subGraph[i][j] = 0;
			else {
				subGraph[i][j] = adjMat[points[i].id][points[j].id];
				subGraph[j][i] = subGraph[i][j];
			}
		}
	}
}

void Cluster::setClusterSize(int size)
{
	this->clusterSize = size;
}

void Cluster::initCluster(vector<ClusterPoint> points)
{
	/*int clusterSize = (int)points.size();
	this->subGraph = new double*[clusterSize];

	for(int i=0;i<clusterSize;++i)
		this->subGraph[i] = new double[clusterSize];

	for (int i = 0; i < clusterSize; ++i) {		
		for (int j = 0; j < clusterSize; ++j) {
			if (i < j)
				subGraph[i][j] = MiningTool::miningDistance(points[i], points[j]);
			else if (i == j)
				subGraph[i][i] = 0;
			else
				subGraph[i][j] = subGraph[j][i];
		}
	}
	this->clusterSize = clusterSize;*/
}

double Cluster::getEC()
{
	double tempEC = 0;
	for (int i = 0; i < clusterSize; ++i) {
		for (int j = 0; j < clusterSize; ++j) {
			if (i < j)
				tempEC += subGraph[i][j];
		}
	}
	return tempEC/clusterSize;
}

Cluster::Cluster()
{
}


Cluster::~Cluster()
{
	{
		vector<ClusterPoint> tmp = points;
		points.swap(tmp);
	}
	if (subGraph == nullptr)
		return;
	clusterSize = (int)points.size();
	for (int i = 0; i < clusterSize; ++i)
		delete[] subGraph[i];
	delete[]subGraph;
	
}
