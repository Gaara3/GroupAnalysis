#ifndef __DEFINE_H__
#define __DEFINE_H__


#define CHAR_SIZE 255

//一次批量插入最大数
#define SQL_NUM 10000

//π的值
#define M_PI 3.14159265358979323846

//地球半径，单位米
#define Earth_Radius 6371004


enum AreaType
{
	AreaType_Unknown = 0,
	AreaType_Rectangle,		//矩形
	AreaType_Circular,		//圆形
	AreaType_PreDef			//预定义区域
};


/**
* @brief 连接mysql数据库的信息
*/
typedef struct _MysqlInfo
{
	char			host[CHAR_SIZE];    //主机
	char			user[CHAR_SIZE];    //用户名
	char			passwd[CHAR_SIZE];  //密码
	char			db[CHAR_SIZE];		//数据库
	unsigned int	port;				//端口号
	unsigned long	client_flag;		// client_flag的值通常为0
	char			unix_socket[CHAR_SIZE];

	_MysqlInfo()
	{
		port = 0;
		client_flag = 0;
		memset(host, 0, sizeof(char) * CHAR_SIZE);
		memset(user, 0, sizeof(char) * CHAR_SIZE);
		memset(passwd, 0, sizeof(char) * CHAR_SIZE);
		memset(unix_socket, 0, sizeof(char) * CHAR_SIZE);
	}
}MysqlInfo;



/**
* @brief 用户筛选条件信息表
*/
typedef struct _SelFilterRule
{
	char			GUID[CHAR_SIZE];		//唯一标识符
	int				Id;						//自增编号
	int				TargetId;				//目标编号
	char			ShipBoardNum[CHAR_SIZE];//舰舷号
	char			TargetModelNum[CHAR_SIZE];//目标型号
	char			TargetName[CHAR_SIZE];	//目标名称
	char			TargetType[CHAR_SIZE];	//目标类型
	char			TargetAttr[CHAR_SIZE];	//目标属性，敌方、我方、友方、中立方、不明
	char			TargetAttrFlag[CHAR_SIZE];//目标作用域，海、陆、空
	int				Militarycivil;			//目标性质，军事、民用
	char			GB[CHAR_SIZE];			//国别
	char			Source[CHAR_SIZE];		//来源
	char			StartTime[CHAR_SIZE];	//起始时间
	char			EndTime[CHAR_SIZE];		//结束时间
	char			Operator[CHAR_SIZE];	//操作员
	char			Reserve1[CHAR_SIZE];	//预留1
	char			Reserve2[CHAR_SIZE];	//预留2

	_SelFilterRule()
	{
		Id = 0;
		TargetId = 0;
		Militarycivil = 0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(ShipBoardNum, 0, sizeof(char) * CHAR_SIZE);
		memset(TargetModelNum, 0, sizeof(char) * CHAR_SIZE);
		memset(TargetName, 0, sizeof(char) * CHAR_SIZE);
		memset(TargetType, 0, sizeof(char) * CHAR_SIZE);
		memset(TargetAttr, 0, sizeof(char) * CHAR_SIZE);
		memset(TargetAttrFlag, 0, sizeof(char) * CHAR_SIZE);
		memset(GB, 0, sizeof(char) * CHAR_SIZE);
		memset(Source, 0, sizeof(char) * CHAR_SIZE);
		memset(StartTime, 0, sizeof(char) * CHAR_SIZE);
		memset(EndTime, 0, sizeof(char) * CHAR_SIZE);
		memset(Operator, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}SelFilterRule;



/**
* @brief 用户选定区域信息表
*/
typedef struct _SelArea
{
	char			GUID[CHAR_SIZE];		//唯一标识符
	int				Id;						//自增编号
	int				SelBatchNum;			//选择批号
	char			RecTime[CHAR_SIZE];		//记录时间
	AreaType		AreaShape;				//区域形状 1.矩形 2.圆形 3.预定义
	int				AreaId;					//区域ID
	char			Operator[CHAR_SIZE];	//操作员
	char			Reserve1[CHAR_SIZE];	//预留1
	char			Reserve2[CHAR_SIZE];	//预留2

	_SelArea()
	{
		Id = 0;
		SelBatchNum = 0;
		AreaShape = AreaType_Unknown;
		AreaId = 0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(RecTime, 0, sizeof(char) * CHAR_SIZE);
		memset(Operator, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}SelArea;



/**
* @brief 轨迹分段数据主表
*/
typedef struct _TrackMain
{
	char			GUID[CHAR_SIZE];		//唯一标识符
	int				TrackId;				//轨迹ID
	int				ContainsPoints;			//包含点数
	int				TargetId;				//目标编号
	char			StartTime[CHAR_SIZE];	//起始时间
	char			EndTime[CHAR_SIZE];		//结束时间
	double			TrackLength;			//轨迹长度
	char			Source[CHAR_SIZE];		//来源
	char			TaskInfo[CHAR_SIZE];	//任务
	double			ConfidenceLevel;		//置信度
	char			Operator[CHAR_SIZE];	//操作员
	char			Reserve1[CHAR_SIZE];	//预留1
	char			Reserve2[CHAR_SIZE];	//预留2

	_TrackMain()
	{
		TrackId = 0;
		ContainsPoints = 0;
		TargetId = 0;
		TrackLength = 0.0;
		ConfidenceLevel = 0.0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(StartTime, 0, sizeof(char) * CHAR_SIZE);
		memset(EndTime, 0, sizeof(char) * CHAR_SIZE);
		memset(Source, 0, sizeof(char) * CHAR_SIZE);
		memset(TaskInfo, 0, sizeof(char) * CHAR_SIZE);
		memset(Operator, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}TrackMain;



/**
* @brief 轨迹分段数据子表
*/
typedef struct _TrackSub
{
	char			GUID[CHAR_SIZE];		//唯一标识符
	int				Id;						//自增编号
	int				TrackId;				//轨迹ID
	int				OrderNum;				//顺序号，在轨迹中的排序号
	char			Time[CHAR_SIZE];		//轨迹点时间
	int				PosixTime;				//POSIX时间，从1970.01.01 00:00:00到当前轨迹点时间的秒数
	char			Source[CHAR_SIZE];		//来源
	double			CentralLongitude;		//中心经度
	double			CentralLatitude;		//中心纬度
	double			CentralAltitude;		//中心高度
	double			UpperLeftLongitude;		//左上经度
	double			UpperLeftLatitude;		//左上纬度
	double			UpperLeftAltitude;		//左上高度
	double			UpperRightLongitude;	//右上经度
	double			UpperRightLatitude;		//右上纬度
	double			UpperRightAltitude;		//右上高度
	double			LowerRightLongitude;	//右下经度
	double			LowerRightLatitude;		//右下纬度
	double			LowerRightAltitude;		//右下高度
	double			LowerLeftLongitude;		//左下经度
	double			LowerLeftLatitude;		//左下纬度
	double			LowerLeftAltitude;		//左下高度
	double			Speed;					//航速
	double			Angle;					//角度
	double			ConfidenceLevel;		//置信度
	char			Reserve1[CHAR_SIZE];	//预留1
	char			Reserve2[CHAR_SIZE];	//预留2

	_TrackSub()
	{
		Id = 0;
		TrackId = 0;
		OrderNum = 0;
		PosixTime = 0;
		CentralLongitude = 0.0;
		CentralLatitude = 0.0;
		CentralAltitude = 0.0;
		UpperLeftLongitude = 0.0;
		UpperLeftLatitude = 0.0;
		UpperLeftAltitude = 0.0;
		UpperRightLongitude = 0.0;
		UpperRightLatitude = 0.0;
		UpperRightAltitude = 0.0;
		LowerRightLongitude = 0.0;
		LowerRightLatitude = 0.0;
		LowerRightAltitude = 0.0;
		LowerLeftLongitude = 0.0;
		LowerLeftLatitude = 0.0;
		LowerLeftAltitude = 0.0;
		ConfidenceLevel = 0.0;
		Speed = 0.0;
		Angle = 0.0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(Time, 0, sizeof(char) * CHAR_SIZE);
		memset(Source, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}TrackSub;


/**
* @brief 轨迹分段数据子表，线段形式
*/
typedef struct _TrackSubGeom
{
	char			GUID[CHAR_SIZE];		//唯一标识符
	int				Id;						//自增编号
	int				TrackId;				//轨迹ID
	int				OrderNum;				//顺序号，起点在轨迹中的排序号
	char			StartTime[CHAR_SIZE];	//起点的时间
	char			EndTime[CHAR_SIZE];		//终点的时间
	int				StartPosixTime;			//起点的POSIX时间，从1970.01.01 00:00:00到当前轨迹点时间的秒数
	int				EndPosixTime;			//终点的POSIX时间，从1970.01.01 00:00:00到当前轨迹点时间的秒数
	char			Source[CHAR_SIZE];		//来源
	double			StartLongitude;			//起点的经度
	double			StartLatitude;			//起点的纬度
	double			EndLongitude;			//终点的经度
	double			EndLatitude;			//终点的纬度
	double			ConfidenceLevel;		//置信度
	char			Reserve1[CHAR_SIZE];	//预留1
	char			Reserve2[CHAR_SIZE];	//预留2

	_TrackSubGeom()
	{
		Id = 0;
		TrackId = 0;
		OrderNum = 0;
		StartPosixTime = 0;
		EndPosixTime = 0;
		StartLongitude = 0.0;
		StartLatitude = 0.0;
		EndLongitude = 0.0;
		EndLatitude = 0.0;
		ConfidenceLevel = 0.0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(StartTime, 0, sizeof(char) * CHAR_SIZE);
		memset(EndTime, 0, sizeof(char) * CHAR_SIZE);
		memset(Source, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}TrackSubGeom;



/**
* @brief 轨迹分段数据抽稀子表
*/
typedef struct _TrackSamSub
{
	char			GUID[CHAR_SIZE];		//唯一标识符
	int				Id;						//自增编号
	int				TrackId;				//轨迹ID
	int				OrderNum;				//顺序号，抽稀之后在轨迹中的排序号
	int				OriginOrderNum;			//顺序号，抽稀之前在轨迹中的排序号
	char			Time[CHAR_SIZE];		//轨迹点时间
	int				PosixTime;				//POSIX时间，从1970.01.01 00:00:00到当前轨迹点时间的秒数
	int				SamplingLevel;			//抽稀级别
	char			Source[CHAR_SIZE];		//来源
	double			CentralLongitude;		//中心经度
	double			CentralLatitude;		//中心纬度
	double			CentralAltitude;		//中心高度
	double			Speed;					//航速
	double			Angle;					//角度
	double			ConfidenceLevel;		//置信度
	char			Reserve1[CHAR_SIZE];	//预留1
	char			Reserve2[CHAR_SIZE];	//预留2

	_TrackSamSub()
	{
		Id = 0;
		TrackId = 0;
		OrderNum = 0;
		OriginOrderNum = 0;
		PosixTime = 0;
		SamplingLevel = 0;
		CentralLongitude = 0.0;
		CentralLatitude = 0.0;
		CentralAltitude = 0.0;
		ConfidenceLevel = 0.0;
		Speed = 0.0;
		Angle = 0.0;
		memset(GUID, 0, sizeof(char) * CHAR_SIZE);
		memset(Time, 0, sizeof(char) * CHAR_SIZE);
		memset(Source, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve1, 0, sizeof(char) * CHAR_SIZE);
		memset(Reserve2, 0, sizeof(char) * CHAR_SIZE);
	}
}TrackSamSub;


#endif //__DEFINE_H__
