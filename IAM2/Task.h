#pragma once

class Task
{
public:
	virtual void updata() = 0;
	virtual bool isAlive()const = 0;
	virtual ~Task(){}
};