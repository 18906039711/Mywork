#pragma once
#ifndef _ControllerListener_H_
#define _ControllerListener_H_
#include"cocos2d.h"
USING_NS_CC;

//������
class ControllerListener :public Node {
public:
	//���ü�������
	void setControllerListener(ControllerListener* controllerListener);

protected:
	ControllerListener* my_controllerListener;
};


#endif