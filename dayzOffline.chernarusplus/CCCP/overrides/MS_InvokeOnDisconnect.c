/*
	DayZ SA CCCP Mod
	
	FileName: MS_InvokeOnConnect.c
	
	Usage:
	FNR:Description

	Authors: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/
    override void InvokeOnDisconnect(PlayerBase player)
    {
        Debug.Log("InvokeOnDisconnect:"+this.ToString(), "Connect");

        if(player) player.OnDisconnect();
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SyncEvents.SendPlayerList, 500); // Send list of players at all clients
        if (m_AdminList.Contains(player.GetIdentity().GetId()))
        {
            int indexA = m_Admins.Find(player);
            m_Admins.Remove(indexA);
        }
    }