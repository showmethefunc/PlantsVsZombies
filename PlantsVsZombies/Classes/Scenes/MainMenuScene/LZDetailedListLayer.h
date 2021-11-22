/**
 *Copyright (c) 2021 LZ.All Right Reserved
 *Author : LZ
 *Date: 2021.11.20
 *Email: 2117610943@qq.com
 */
#pragma once
#include "cocos2d.h"
#include "Based/LZDialog.h"

using namespace cocos2d;

class DetailedList:public Dialog
{
public:
	CREATE_FUNC(DetailedList);

CC_CONSTRUCTOR_ACCESS:
	DetailedList();
	~DetailedList();
	virtual bool init() override;

private:
	void onShowChart(const string& gName, const Vec2& gPos,
		string* mName, Vec2* mPos,
		string* mNumber, Vec2* mNPos,
		Vec2* pos, float* number, Color4F* color, int id = 2);

	int getWorldMostLevel(bool dif, int id);
	void onShowBackButton();
	void onShowOtherThings();
	void onShowTextData(const string& name, const string& name2, const Vec2& pos, const Vec2& pos2, int size = 25, int size2 = 25);
	void onShowSumList();

private:
	DrawNode* _drawNode;
	int _modernPLevel;
	int _modernKLevel;
	int _bigMapPLevel;
	int _bigMapKLevel;
	int _hammerLevel;
	int _testGroundLevel;
	int _izeLevel;
};