#pragma once
#include <string>

class Profile
{
public:
	Profile(int number);
	~Profile();
	
	//////////////////////////////////////////////
	//////////////// Accessors //////////////////
	std::string GetFileName(void) const { return m_sFileName; }
	std::string GetTime(void) const { return m_sTime; }

	std::string GetCurrentLevel(void) const { return m_sCurrentLevel; }
	int GetCollectedHonor(void) const { return m_unCollectedHonor; }
	int GetProfileNumber(void) const { return m_nNumber; }

	////////////////////////////////////////////
	//////////////// Mutators //////////////////
	void SetHonorCollected(int honor) { m_unCollectedHonor = honor; }
	void SetCurrentLevel(std::string name) { m_sCurrentLevel = name; }
	void SetTime(std::string time) { m_sTime =  time; }


private:

	std::string m_sFileName;
	int m_unCollectedHonor;
	std::string m_sCurrentLevel;
	int m_nNumber;
	std::string m_sTime ="";
};

