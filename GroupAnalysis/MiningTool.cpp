#include "MiningTool.h"
#include <math.h>


MiningTool::MiningTool()
{
}


MiningTool::~MiningTool()
{
}

double MiningTool::miningDistance(OriginPoint a, OriginPoint b) {
	int tDiff = abs(a.getPosixtime() - b.getPosixtime());
	double distance = distanceBetweenPoints(a.getLongitude(), a.getLatitude(), b.getLongitude(), b.getLatitude());
	double angleDiff = abs(a.getAngle() - b.getAngle());
	double speedDiff = abs(a.getSpeed() - b.getSpeed());
	int sameTarget = a.getTargetID() == b.getTargetID() ? 0 : 1;

}


double MiningTool::distanceBetweenPoints(double lastLongitude, double lastLatitude, double longitude, double latitude) {
	double res = 0;
	if (lastLatitude <= 90 && lastLongitude <= 180) {	//当一段轨迹完结，设置异常值以便于新一段计算
		double p = 0.017453292519943295;    // Math.PI / 180
		double a = 0.5 - cos((latitude - lastLatitude) * p) / 2 + cos(latitude * p) * cos(lastLatitude * p) *(0.5 - cos((longitude - lastLongitude) * p) / 2);

		res = 12742 * asin(sqrt(a)); // 2 * R; R = 6371 km
	}
	return res;
}