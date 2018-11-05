/*
	DayZ SA CCCP Mod
	
	FileName: CreateLandObject.c
	
	Usage:
	FNR:Description

	Authors: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/

Object CreateLandObject(string objectName, vector pos, vector rot, Object obj) {
    obj = GetGame().CreateObject(objectName, "0 0 0");
    obj.SetPosition(pos);
    obj.SetOrientation(rot);
	return obj;
}