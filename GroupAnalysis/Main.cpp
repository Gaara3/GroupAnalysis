#include "TrackPoint.h"
#include <vector>

void readTrackPointFromDB(vector<TrackPoint> &);	//��ȡhistory������е��¼
void readTrackPointFromDB(vector<TrackPoint> &, char*);	//��ȡ�ض������ĵ��¼
int** clusterGenerator(vector<TrackPoint>, int timeInterval);//��vector�ڵĵ㼣��timeIntervalΪ��λ����

int main() {
	vector<TrackPoint> Points;
	int timeInterval = 600;	//TODO read from config
	readTrackPointFromDB(Points);
	
	int **clusterInfo = clusterGenerator(Points, timeInterval);
}