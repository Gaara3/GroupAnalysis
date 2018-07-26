#include "TrackPoint.h"
#include <vector>

void readTrackPointFromDB(vector<TrackPoint> &);	//读取history里的所有点记录
void readTrackPointFromDB(vector<TrackPoint> &, char*);	//读取特定条件的点记录
int** clusterGenerator(vector<TrackPoint>, int timeInterval);//对vector内的点迹以timeInterval为单位聚类

int main() {
	vector<TrackPoint> Points;
	int timeInterval = 600;	//TODO read from config
	readTrackPointFromDB(Points);
	
	int **clusterInfo = clusterGenerator(Points, timeInterval);
}