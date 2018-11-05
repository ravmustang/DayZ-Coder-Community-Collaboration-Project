/*
	DayZ SA CCCP Mod
	
	FileName: Init.c
	
	Usage:
	FNR:Description

	Authors: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	FNR:LicenseInfo
*/

void CreateLandObject(string objectName, vector pos, vector rot, Object obj) {
    obj = GetGame().CreateObject(objectName, "0 0 0");
    obj.SetPosition(pos);
    obj.SetOrientation(rot);
}