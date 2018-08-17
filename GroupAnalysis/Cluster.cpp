#include "Cluster.h"
#include "MiningTool.h"
#include "Tools.h"

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
	updateEC();
}

Cluster::Cluster(const Cluster & c)
{
	this->clusterSize = c.clusterSize;
	this->points.assign(c.points.begin(), c.points.end());
	this->subGraph = new double*[clusterSize];
	for (int i = 0; i < clusterSize; ++i) {
		subGraph[i] = new double[clusterSize];
		for (int j = 0; j < clusterSize; ++j) 
			subGraph[i][j] = c.subGraph[i][j];
	}
		
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
	Tools::writeArray2File("subGraph.csv", subGraph, clusterSize, clusterSize);
}

void Cluster::setClusterSize(int size)
{
	this->clusterSize = size;
}

void Cluster::updateClusterSize()
{
	setClusterSize((int)points.size());
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

void Cluster::updateEC()
{
	double tempEC = 0;
	for (int i = 0; i < clusterSize; ++i) {
		for (int j = 0; j < clusterSize; ++j) {
			if (i < j)
				tempEC += subGraph[i][j];
		}
	}
	this->EC= tempEC/clusterSize;
}

void Cluster::updateClusterInfo(double** adjMat)
{
	resetSubGraph();
	updateClusterSize();
	setSubGraph(adjMat);
	updateEC();
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
