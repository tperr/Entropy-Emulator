#pragma once
#include <fstream>
#include <iostream>

using namespace std;

// abstract class for each device, used for a few different classes
class Device
{
// private stuff, never used
private:
// stuff that classes themselves should use
protected:
	string deviceName;
	virtual void dump();
	virtual void reset() = 0;
// stuff that everything should use
public:
	virtual void doCycleWork() = 0;
	virtual void parse(ifstream& file) = 0;
	virtual void finish() = 0;

	string getName() { return deviceName; }

	
	
};