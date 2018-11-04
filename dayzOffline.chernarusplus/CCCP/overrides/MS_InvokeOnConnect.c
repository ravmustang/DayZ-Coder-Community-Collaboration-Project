/*
	DayZ SA CCCP Mod
	
	FileName: MS_InvokeOnConnect.c
	
	Usage:
	FNR:Description

	Authors: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	FNR:LicenseInfo
*/
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		identityName = identity.GetName();
		identityID = identity.GetId();
		Debug.Log("InvokeOnConnect:"+this.ToString(),"Connect");

		if( player ) player.OnConnect();

		// Send list of players at all clients
		SyncEvents.SendPlayerList();

		g_Game.GetProfileString("JoinedServer"+identityID, joinedServer);
		if( joinedServer == "false") 
		{
			msg = "DirtyS JoinFeed: " + identityName + " just logged on!";
			ServerMessagingSystem(msg);
			
			g_Game.SetProfileString("joinedServerMessageStatus"+identityID, "true");
			g_Game.SaveProfile();
		}else{
			string msg = "DirtyS JoinFeed: " + identityName + " has respawned!";
			ServerMessagingSystem(msg);
		}

		if ( m_AdminList.Contains( identityID ) ) admins.Insert(player);
		
		if( identityName == "Survivor" ) survivors.Insert(player);
	}
