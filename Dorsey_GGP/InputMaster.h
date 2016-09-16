#pragma once
#include<Windows.h>
class InputMaster
{
public:
	///<summary>Gives the instance of the master, and if none exists, creates one first. </summary>
	static InputMaster* GetInstance(void);
	///<summary>Destroys the instance of the master.</summary>
	static void ReleaseInstance(void);
private:
	///<summary> Private constructor. Gets singleton instances, sets up input settings, and binds callback functions.</summary>
	InputMaster(void);
	///<summary> Private copy constructor. Does nothing, just can't be called.</summary>
	InputMaster(const InputMaster &other);
	///<summary> Private assignment operator. Does nothing, just can't be called.</summary>
	InputMaster& operator=(const InputMaster &other);
	///<summary> Private destructor. Does nothing, just can't be called.</summary>
	~InputMaster(void);


};


