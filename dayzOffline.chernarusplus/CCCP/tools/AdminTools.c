/*
	DayZ SA CCCP Mod
	
	FileName: AdminTools.c
	
	Usage:
	FNR:Description

	Authors: DaOne
	Modified by: DayZ SA CCCP Mod Team and Community Contributors(see credits.md)
	FNR:ModTeamInfo
	
	This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/   
        
    //------------------------------Admin quick Commands (Chatbased)-------------------
    int TeleportAllPlayersTo(PlayerBase PlayerAdmin)
    {
        //Collectes Arry of players
        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);
        vector AdminPos;
        AdminPos = PlayerAdmin.GetPosition();
        for (int targetloop = 0; targetloop < players.Count(); targetloop++)
        {
            PlayerBase Target = players.Get(targetloop);
            Target.SetPosition(AdminPos);
        }
        return targetloop;
    }

    void oSpawnItemFunc(bool ground, PlayerBase player, string ClassName)
    {
        EntityAI item;
        ItemBase itemBs;
        vector NewPosition;
        vector OldPosition;
        if (ground)
        {
            OldPosition = player.GetPosition();
            NewPosition[0] = OldPosition[0] + 1.5;
            NewPosition[1] = OldPosition[1] + 0.1;
            NewPosition[2] = OldPosition[2] + 1.5;
            item = GetGame().CreateObject(ClassName, NewPosition, false, true);
        }
        else
        {
            item = player.GetInventory().CreateInInventory(ClassName);
            itemBs = ItemBase.Cast(item);    
            itemBs.SetQuantity(1);
        }
    }

    vector SnapToGround(vector pos)
    {
        float pos_x = pos[0];
        float pos_z = pos[2];
        float pos_y = GetGame().SurfaceY(pos_x, pos_z);
        vector tmp_pos = Vector(pos_x, pos_y, pos_z);
        tmp_pos[1] = tmp_pos[1] + pos[1];
        return tmp_pos;
    }
    
    void addMags(PlayerBase player, string mag_type, int count)
    {
        if (count < 1)
            return;
        EntityAI mag;
        for (int i = 0; i < count; i++)
        {
            mag = player.GetInventory().CreateInInventory(mag_type);
        }
        player.SetQuickBarEntityShortcut(mag, 1, true);
    }
    
    vector GetCursorPos(PlayerBase player)
    {
        vector rayStart = GetGame().GetCurrentCameraPosition();
        vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;
        vector hitPos;
        vector hitNormal;
        int hitComponentIndex;
        DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, player);
        return hitPos;
    }
    
    void teleport(PlayerBase player, vector pos)
    {
        float distance = vector.Distance(player.GetPosition(), pos);
        if (distance < 5000)
        {
            EntityAI oVehicle = player.GetDrivingVehicle();
            if (oVehicle)
            {
                string msg = "Get out of the vehicle first!";
                Param1<string> Msgparam = new Param1<string>(msg);
                GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, player.GetIdentity());
            }
            else
            {
                player.SetPosition(pos);
            }
        }
        else
        {
            msg = "Distance for teleportation is too far!";
            GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, player.GetIdentity());
        }
    }
    
    void AdminMessaging(PlayerBase requestingAdmin, string AdminMsg)
    {
        Param1<string> m_MessageParam = new Param1<string>(AdminMsg);
        if (AllAdminsMsg)
        {
            for (int ad = 0; ad < m_Admins.Count(); ad++)
            {
                Man currAdmin = m_Admins.Get(ad);
                if (currAdmin) GetGame().RPCSingleParam(currAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, currAdmin.GetIdentity());
            }
        }
        else
        {
            GetGame().RPCSingleParam(requestingAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, requestingAdmin.GetIdentity());
        }
    }
    
    Man FindSelectedPlayer(string pName) {
        matchcounter = 0;
        PlayerBase SelectedPlayer = NULL;
        array<Man> players = new array<Man>;
        GetGame().GetPlayers(players);
        for (int zm = 0; zm < players.Count(); zm++)
        {
            PlayerBase currentPlayer = players.Get(zm);
            PlayerIdentity SelectedIdentity = currentPlayer.GetIdentity();
            string SelectedPlayerName = SelectedIdentity.GetName();
            int numUpperSelected = SelectedPlayerName.ToLower();
            int numUpperpName = pName.ToLower();
            if (SelectedPlayerName.Contains(pName))
            {
                SelectedPlayer = currentPlayer;
                matchcounter++;
            }
        }
        return SelectedPlayer;
    }

	void HideMessages()
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );

		for (int hideloop = 0; hideloop < players.Count(); hideloop++)
		{
			PlayerBase hidePlayer = players.Get(hideloop);
            if (!m_AdminList.Contains(hidePlayer.GetIdentity().GetName()))
            {
                for (int hideloop2 = 0; hideloop2 < 12; hideloop2++)
                {
                    Param1<string> Msgparam;
                    Msgparam = new Param1<string>("  ");
                    GetGame().RPCSingleParam(hidePlayer, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, hidePlayer.GetIdentity());
                }
            }
            else
            {
                break;
			}
		}
	}
    
    override void OnEvent(EventType eventTypeId, Param params)
    {
        PlayerIdentity identity;
        PlayerBase player;
        ItemBase item;
        
        //eventTypeId Switch
        switch(eventTypeId)
        {
            //--------------
            case ChatMessageEventTypeID:
                ChatMessageEventParams m_ChatParams = ChatMessageEventParams.Cast(params);
                
                array<Man> m_Players = new array<Man>;
                GetGame().GetPlayers(m_Players);
        
                // Variables for Command Parsing
                array<string> m_CmdParams = new array<string>;
                string m_CodeLine, m_MessageOut, m_LocationName;
                string m_LineIn = m_ChatParams.param3;
                m_LineIn.Split(" ", m_CmdParams);
                
                // Variables for Commands
                vector m_FixedPosition, m_RawPosition;
                PlayerBase m_SelectedPlayer;
                PlayerIdentity m_SelectedIdentity;
                Param1<string> m_MsgParam;
                int counter = 0;
                
                //Print("Channel: "+m_ChatParams.param1+" From: "+m_ChatParams.param2+ " Message: "+m_ChatParams.param3+ " Color: "+m_ChatParams.param4);
                //PrintFormat("[DonkeyDebug] - missionServer.c ChatMessageEventTypeID m_ChatParams:%1 - param1:%2 - param2:%3 - param3:%4 ", m_ChatParams, m_ChatParams.param1, m_ChatParams.param2, m_ChatParams.param3);
                //if (m_CmdParams[0] == "/") { Print("Command Sent: " + m_CmdParams[0] + m_CmdParams[1] + m_CmdParams[2] + m_CmdParams[3] + m_CmdParams[4] + m_CmdParams[5] + m_CmdParams[6] + m_CmdParams[7] + m_CmdParams[8] + m_CmdParams[9] + "."); } //Debugging

                // Admin Loop
                for (int adminsloop = 0; adminsloop < m_Admins.Count(); adminsloop++)
                {
                    PlayerBase m_Admin = m_Admins.Get(adminsloop);
                    if ((m_Admin == NULL)|| !m_Admin) { break; }
                    PlayerIdentity m_PlayerAdminIdentity = m_Admin.GetIdentity();
                    string m_AdminUID = m_PlayerAdminIdentity.GetId();
                    string m_AdminName = m_Admin.GetIdentity().GetName();
                    // Check player name to sent chat name
                    if (m_AdminName == m_ChatParams.param2 && m_AdminList.Contains(m_AdminUID))
                    {
                        // Check that Command is not empty
                        if (m_CmdParams[0] != "")
                        {
                            if (!m_CmdParams[0].Contains("/")) { break; }
                            //GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.HideMessages, 1, false);
                            
                            // Command Switch
                            switch (m_CmdParams[0])
                            {
                                // -------------------------------------------------------------------------------------------------------------------------- START OF COMMANDS =================
                                // m_CmdParams[0]                          (command sent by user)
                                // m_CmdParams[1]                          (parameter 1 for commands)
                                // m_CmdParams[2]                          (parameter 2 for commands)
                                // m_CmdParams[3]                          (parameter 3 for commands)
                                // m_CmdParams[4]                          (parameter 4 for commands)
                                // m_CmdParams[5]                          (parameter 5 for commands)
                                // m_CmdParams[6]                          (parameter 6 for commands)
                                // m_CmdParams[7]                          (parameter 7 for commands)
                                // m_CmdParams[8]                          (parameter 8 for commands)
                                // m_CmdParams[9]                          (parameter 9 for commands)
                                // --------------------------------------------------------------------------------------------------------------------------
                    //TELEPORT TOOLS
                                // -------------------------------------------------------------------------------------------------------------------------- /teleport
                                case "/teleport":
                                    vector hitPos = GetCursorPos(m_Admin);
                                    teleport(m_Admin, hitPos);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tppm
                                case "/tppm":
                                    if (m_CmdParams[1] != "")
                                    {
                                        m_SelectedPlayer = FindSelectedPlayer(m_CmdParams[1]);
                                        if (matchcounter == 1) {
                                            if (m_SelectedPlayer != NULL)
                                            {
                                                m_Admin.SetPosition(m_SelectedPlayer.GetPosition());
                                                m_MessageOut = "Admin:" + m_AdminName + " was teleported to " + m_SelectedPlayer.GetIdentity().GetName() + ".";
                                            }
                                            else
                                            {
                                                m_MessageOut = "Could not find player " + m_CmdParams[1] + ". Please check the name and try again.";
                                            }
                                        }
                                        else if (matchcounter > 1)
                                        {
                                            m_MessageOut = "Multiple matches to : " + m_CmdParams[1] + ". Please be more specific.";
                                        }
                                        else
                                        {
                                            m_MessageOut = "No matches to : " + m_CmdParams[1] + " found.";
                                        }
                                    }
                                    else
                                    {
                                        m_MessageOut = "Please type a player name first.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tplist
                                case "/tplist":
                                    for (counter = 0; counter < m_TPLocations.Count(); counter++)
                                    {
                                        if (counter == 0)
                                        {
                                            m_MessageOut = m_TPLocations.GetKey(counter);
                                        }
                                        else
                                        {
                                            m_MessageOut = m_MessageOut + ", " + m_TPLocations.GetKey(counter);
                                        }
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tpto
                                case "/tpto":
                                    if (m_CmdParams[1] != "")
                                    {
                                        m_LocationName = m_CmdParams[1];
                                        for (counter = 2; counter < m_CmdParams.Count(); counter++)
                                        {
                                            if (m_CmdParams[counter] != "")
                                            { 
                                                m_LocationName = m_LocationName + " " + m_CmdParams[counter];
                                            }
                                            else { break; }
                                        }
                                        Print(m_LocationName);
                                    
                                        if (m_TPLocations.Contains(m_LocationName))
                                        {
                                            m_TPLocations.Find(m_LocationName, m_RawPosition);
                                            m_FixedPosition[0] = m_RawPosition[0];
                                            m_FixedPosition[2] = m_RawPosition[2];
                                            m_FixedPosition = SnapToGround(m_FixedPosition);
                                            m_Admin.SetPosition(m_FixedPosition);
                                            m_MessageOut = "Teleported To Location: " + m_LocationName + ".";
                                        }
                                        else
                                        {
                                            m_MessageOut = "Teleport Failed! Location: " + m_LocationName + " is not on the list!"; }
                                    }
                                    else
                                    {
                                        m_MessageOut = "Teleport Failed! Please enter a location first!";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tpc
                                case "/tpc":
                                    if (m_CmdParams[1] != "" && m_CmdParams[2] != "")
                                    {
                                        m_FixedPosition[0] = m_CmdParams[1].ToFloat();
                                        m_FixedPosition[1] = 0;
                                        m_FixedPosition[2] = m_CmdParams[2].ToFloat();
                                        m_FixedPosition = SnapToGround(m_FixedPosition);
                                        m_Admin.SetPosition(m_FixedPosition);
                                        m_MessageOut = "Teleported to: " + m_FixedPosition;
                                    }
                                    else
                                    {
                                        m_MessageOut = "Teleport Failed. Make sure to provide X and Z coordinates.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tpp
                                case "/tpp":
                                    if (m_CmdParams[1] != "")
                                    {
                                        m_SelectedPlayer = FindSelectedPlayer(m_CmdParams[1]);
                                        if (matchcounter == 1) {
                                            if (m_SelectedPlayer != NULL)
                                            {
                                                m_SelectedPlayer.SetPosition(m_Admin.GetPosition());
                                                m_MessageOut = "Player " + m_SelectedPlayer.GetIdentity().GetName() + " was teleported to Admin: " + m_AdminName + "'s location!";
                                            }
                                            else
                                            {
                                                m_MessageOut = "Could not find player " + m_CmdParams[1] + ". Please check the name and try again.";
                                            }
                                        }
                                        else if (matchcounter > 1)
                                        {
                                            m_MessageOut = "Multiple matches to : " + m_CmdParams[1] + ". Please be more specific.";
                                        }
                                        else
                                        {
                                            m_MessageOut = "No matches to : " + m_CmdParams[1] + " found.";
                                        }
                                    }
                                    else
                                    {
                                        m_MessageOut = "Please type a player name first.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tppto
                                case "/tppto":
                                    if (m_CmdParams[1] != "")
                                    {
                                        m_LocationName = m_CmdParams[2];
                                        for (counter = 3; counter < m_CmdParams.Count(); counter++)
                                        {
                                            if (m_CmdParams[counter] != "")
                                            {
                                                m_LocationName = m_LocationName + " " + m_CmdParams[counter];
                                            }
                                            else { break; }
                                        }
                                        Print(m_LocationName);
                                        if (m_LocationName != "")
                                        {
                                            m_SelectedPlayer = FindSelectedPlayer(m_CmdParams[1]);
                                            if (matchcounter == 1) {
                                                if (m_SelectedPlayer != NULL)
                                                {
                                                    if (m_TPLocations.Contains(m_LocationName))
                                                    {
                                                        m_TPLocations.Find(m_LocationName, m_RawPosition);
                                                        m_FixedPosition[0] = m_RawPosition[0];
                                                        m_FixedPosition[2] = m_RawPosition[2];
                                                        m_FixedPosition = SnapToGround(m_FixedPosition);
                                                        m_SelectedPlayer.SetPosition(m_FixedPosition);
                                                        m_MessageOut = m_SelectedPlayer.GetIdentity().GetName() + " has been teleported to: " + m_LocationName + ".";
                                                    }
                                                    else
                                                    {
                                                        m_MessageOut = "Teleport Failed! Location: " + m_LocationName + " is not on the list!";
                                                    }
                                                }
                                                else
                                                {
                                                    m_MessageOut = "Could not find player " + m_CmdParams[1] + ". Please check the name and try again.";
                                                }
                                            }
                                            else if (matchcounter > 1)
                                            {
                                                m_MessageOut = "Multiple matches to : " + m_CmdParams[1] + ". Please be more specific.";
                                            }
                                            else
                                            {
                                                m_MessageOut = "No matches to : " + m_CmdParams[1] + " found.";
                                            }
                                        }
                                        else
                                        {
                                            m_MessageOut = "Please type a Location first.";
                                        }
                                    }
                                    else
                                    {
                                        m_MessageOut = "Please type a player name first.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                    //HELPER TOOLS
                                // -------------------------------------------------------------------------------------------------------------------------- /spi
                                case "/spi":
                                    if (m_CmdParams[1] != "")
                                    {
                                        oSpawnItemFunc(false, m_Admin, m_CmdParams[1]);
                                        m_MessageOut = "Admin: " + m_AdminName + " spawned Item: " + m_CmdParams[1] + " in their Inventory!";
                                    }
                                    else
                                    {
                                        m_MessageOut = "Please type an item name first.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /spg
                                case "/spg":
                                    if (m_CmdParams[1] != "")
                                    {
                                        oSpawnItemFunc(true,m_Admin,m_CmdParams[1]);
                                        m_MessageOut = "Admin: " + m_AdminName + " spawned Item: " + m_CmdParams[1] + " on the ground!";
                                    }
                                    else
                                    {
                                        m_MessageOut = "Please type an item name first.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /heal
                                case "/heal":
                                    m_MsgParam = new Param1<string>("Player Healed!");
                                    GetGame().RPCSingleParam(m_Admin, ERPCs.RPC_USER_ACTION_MESSAGE, m_MsgParam, true, m_Admin.GetIdentity());
                                    for (counter = 0; counter < m_Admin.GetBleedingManager().m_BleedingSources.Count(); counter++)
                                    {
                                        player.GetBleedingManager().RemoveSingleBleedingSource();
                                    }
                                    m_Admin.SetHealth(m_Admin.GetMaxHealth("", ""));
                                    m_Admin.SetHealth("", "Blood", m_Admin.GetMaxHealth("", "Blood"));
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /healp
                                case "/healp":
                                    if (m_CmdParams[1] != "")
                                    {
                                        m_SelectedPlayer = FindSelectedPlayer(m_CmdParams[1]);
                                        if (matchcounter == 1) {
                                            if (m_SelectedPlayer != NULL)
                                            {
                                                for (counter = 0; counter < m_SelectedPlayer.GetBleedingManager().m_BleedingSources.Count(); counter++)
                                                {
                                                    m_SelectedPlayer.GetBleedingManager().RemoveSingleBleedingSource();
                                                }
                                                m_SelectedPlayer.SetHealth(m_SelectedPlayer.GetMaxHealth("", ""));
                                                m_SelectedPlayer.SetHealth("", "Blood", m_SelectedPlayer.GetMaxHealth("", "Blood"));
                                                m_MessageOut = "Player " + m_SelectedPlayer.GetIdentity().GetName() + " was healed by Admin: " + m_AdminName + ".";
                                            }
                                            else
                                            {
                                                m_MessageOut = "Could not find player " + m_CmdParams[1] + ". Please check the name and try again.";
                                            }
                                        }
                                        else if (matchcounter > 1)
                                        {
                                            m_MessageOut = "Multiple matches to : " + m_CmdParams[1] + ". Please be more specific.";
                                        }
                                        else
                                        {
                                            m_MessageOut = "No matches to : " + m_CmdParams[1] + " found.";
                                        }
                                    }
                                    else
                                    {
                                        m_MessageOut = "Please type a player name first.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /healall
                                case "/healall":
                                    for (int healallloop = 0; healallloop < m_Players.Count(); healallloop++)
                                    {
                                        PlayerBase m_Patient = m_Players.Get(healallloop);
                                            for (counter = 0; counter < m_Patient.GetBleedingManager().m_BleedingSources.Count(); counter++)
                                            {
                                                m_Patient.GetBleedingManager().RemoveSingleBleedingSource();
                                            }
                                        m_Patient.SetHealth(m_Admin.GetMaxHealth("", ""));
                                        m_Patient.SetHealth("","Blood", m_Admin.GetMaxHealth("", "Blood"));
                                    }
                                    m_MessageOut = "All Player's Healed by Admin: " + m_AdminName;
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                    //PUNISHMENT TOOLS
                                // -------------------------------------------------------------------------------------------------------------------------- /killp
                                case "/killp":
                                    if (m_CmdParams[1] != "")
                                    {
                                        m_SelectedPlayer = FindSelectedPlayer(m_CmdParams[1]);
                                        if (matchcounter == 1) {
                                            if (m_SelectedPlayer != NULL)
                                            {
                                                m_SelectedPlayer.SetHealth(0);
                                                m_MessageOut = "Player: " + m_SelectedPlayer.GetIdentity().GetName() + " was Killed by Admin: " + m_AdminName + ".";
                                            }
                                            else
                                            {
                                                m_MessageOut = "Could not find player " + m_CmdParams[1] + ". Please check the name and try again.";
                                            }
                                        }
                                        else if (matchcounter > 1)
                                        {
                                            m_MessageOut = "Multiple matches to : " + m_CmdParams[1] + ". Please be more specific.";
                                        }
                                        else
                                        {
                                            m_MessageOut = "No matches to : " + m_CmdParams[1] + " found.";
                                        }
                                    }
                                    else
                                    {
                                        m_MessageOut = "Please type a player name first.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /strip
                                case "/strip":
                                    if (m_CmdParams[1] != "")
                                        {
                                        m_SelectedPlayer = FindSelectedPlayer(m_CmdParams[1]);
                                        if (matchcounter == 1) {
                                            if (m_SelectedPlayer != NULL)
                                            {
                                                m_SelectedPlayer.RemoveAllItems();
                                                m_MessageOut = "Player:" + m_SelectedPlayer.GetIdentity().GetName() + " was striped down by Admin: " + m_AdminName + ".";
                                            }
                                            else
                                            {
                                                m_MessageOut = "Could not find player " + m_CmdParams[1] + ". Please check the name and try again.";
                                            }
                                        }
                                        else if (matchcounter > 1)
                                        {
                                            m_MessageOut = "Multiple matches to : " + m_CmdParams[1] + ". Please be more specific.";
                                        }
                                        else
                                        {
                                            m_MessageOut = "No matches to : " + m_CmdParams[1] + " found.";
                                        }
                                    }
                                    else
                                    {
                                        m_MessageOut = "Please type a player name first.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /salty
                                case "/salty":
                                    if (m_CmdParams[1] != "")
                                    {
                                        m_SelectedPlayer = FindSelectedPlayer(m_CmdParams[1]);
                                        if (matchcounter == 1) {
                                            if (m_SelectedPlayer != NULL)
                                            {
                                                item = GetGame().CreateObject("M67Grenade", m_SelectedPlayer.GetPosition(), false, true);
                                                item.Explode();
                                                m_MessageOut = "Player:" + m_SelectedPlayer.GetIdentity().GetName() + " was salted(Grenade) by Admin: " + m_AdminName + ".";
                                            }
                                            else
                                            {
                                                m_MessageOut =  "Could not find player " + m_CmdParams[1] + ". Please check the name and try again.";
                                            }
                                        }
                                        else if (matchcounter > 1)
                                        {
                                            m_MessageOut = "Multiple matches to : " + m_CmdParams[1] + ". Please be more specific.";
                                        }
                                        else
                                        {
                                            m_MessageOut = "No matches to : " + m_CmdParams[1] + " found.";
                                        }
                                    }
                                    else
                                    {
                                        m_MessageOut = "Please type a player name first.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /kill
                                case "/kill":
                                    m_Admin.SetHealth(0);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /killall
                                case "/killall":
                                    for (counter = 0; counter < m_Players.Count(); counter++)
                                    {
                                        PlayerBase Target = m_Players.Get(counter);
                                        if ((Target.GetIdentity()) == m_PlayerAdminIdentity){
                                            //DO NOTHING, DON'T KILL THE ADMIN WHO SENT THE COMMAND.
                                        }
                                        else
                                        {
                                            Target = m_Players.Get(counter);
                                            Target.SetHealth(0);
                                        }
                                    }
                                    m_MessageOut = "All Player's Killed by Admin: " + m_AdminName + ".";
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                    //DEVELOPER TOOLS
                                // -------------------------------------------------------------------------------------------------------------------------- /build
                                case "/build":
                                    if (m_CmdParams[1] != "")
                                    {
                                        vector admPOS = m_Admin.GetPosition();
                                        vector buildPOS; 
                                        buildPOS[0] = admPOS[0] + 5;
                                        buildPOS[2] = admPOS[2] + 5;
                                        Object Buildable = GetGame().CreateObject(m_CmdParams[1], buildPOS, false, false, true);
                                        //AttachTo(Object obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0")
                                        //Buildable.AttachTo(m_Admin, "0, 5, 0", "0 0 1");
                                        m_MessageOut = "Object " + m_CmdParams[1] + " created successfully!";
                                    }
                                    else
                                    {
                                        m_MessageOut = "Please type an Object name first.";
                                    }
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /list
                                case "/list":
                                    for (counter = 0; counter < m_Players.Count(); counter++)
                                    {
                                        m_SelectedPlayer = m_Players.Get(counter);
                                        m_SelectedIdentity = m_SelectedPlayer.GetIdentity();
                                        m_MsgParam = new Param1<string>("Player Name: " + m_SelectedIdentity.GetName());
                                        GetGame().RPCSingleParam(m_Admin, ERPCs.RPC_USER_ACTION_MESSAGE, m_MsgParam, true, m_Admin.GetIdentity());
                                    }
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /tpalltome
                                case "/tpalltome":
                                    int tpCount = TeleportAllPlayersTo(m_Admin);
                                    m_MessageOut = "All " + tpCount.ToString() + " Players Teleported to Admin: " + m_AdminName + "'s POS!";
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                        //ADMIN TOOLS
                                // -------------------------------------------------------------------------------------------------------------------------- /godmode
                                case "/godmode":
                                    string currentMode;
                                    g_Game.GetProfileString("adminGodMode" + m_AdminUID, currentMode);
                                    if (currentMode == "false")
                                    {
                                        g_Game.SetProfileString("adminGodMode" + m_AdminUID, "true");
                                        m_MsgParam = new Param1<string>("You have turned Godmode ON");
                                    }
                                    else
                                    {
                                        g_Game.SetProfileString("adminGodMode" + m_AdminUID, "false");
                                        m_MsgParam = new Param1<string>("You have turned Godmode OFF");
                                    }
                                    g_Game.SaveProfile();
                                    GetGame().RPCSingleParam(m_Admin, ERPCs.RPC_USER_ACTION_MESSAGE, m_MsgParam, true, m_Admin.GetIdentity());
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /freecam
                                case "/freecam":
                                    PlayerBase pBody = m_Admin;
                                    if (m_FreeCamera)
                                    {
                                        GetGame().SelectPlayer(m_Admin.GetIdentity(), pBody);
                                        m_FreeCamera = false;
                                        m_MessageOut = "Admin: " + m_AdminName + " is Exiting FreeCam!";
                                        AdminMessaging(m_Admin, m_MessageOut);
                                    }
                                    else
                                    {
                                        GetGame().SelectPlayer(m_Admin.GetIdentity(), NULL);
                                        GetGame().SelectSpectator(m_Admin.GetIdentity(), "freedebugcamera", m_Admin.GetPosition()); //staticcamera, freedebugcamera, dayzspectator
                                        m_FreeCamera = true;
                                        m_MessageOut = "Admin: " + m_AdminName + " is Entering FreeCam!";
                                        AdminMessaging(m_Admin, m_MessageOut);
                                    }
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /remove
                                case "/remove":
                                    EntityAI CurrentInHands = m_Admin.GetHumanInventory().GetEntityInHands();
                                    string displayName = CurrentInHands.ConfigGetString("displayName");
                                    GetGame().ObjectDelete(CurrentInHands);
                                    m_Admin.OnItemInHandsChanged();
                                    m_MessageOut = "Admin: " + m_AdminName + " have removed " + displayName + ".";
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /nighttime
                                case "/nighttime":
                                    GetGame().GetWorld().SetDate(1988, 5, 23, 22, 0);
                                    m_MsgParam = new Param1<string>("NIGHT TIME!!");
                                    GetGame().RPCSingleParam(m_Admin, ERPCs.RPC_USER_ACTION_MESSAGE, m_MsgParam, true, m_Admin.GetIdentity());
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /daytime
                                case "/daytime":
                                    GetGame().GetWorld().SetDate(1988, 5, 23, 12, 0);
                                    m_MsgParam = new Param1<string>("DAY TIME!!");
                                    GetGame().RPCSingleParam(m_Admin, ERPCs.RPC_USER_ACTION_MESSAGE, m_MsgParam, true, m_Admin.GetIdentity());
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /dawntime
                                case "/dawntime":
                                    GetGame().GetWorld().SetDate(1988, 5, 23, 6, 0);
                                    m_MsgParam = new Param1<string>("Time Changed to 6 am");
                                    GetGame().RPCSingleParam(m_Admin, ERPCs.RPC_USER_ACTION_MESSAGE, m_MsgParam, true, m_Admin.GetIdentity());
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /debug
                                case "/debug":
                                    if (m_IsDebugRunning)
                                    {
                                        m_MessageOut = "Admin: " + m_AdminName + " DeBug Monitor (Status Monitor) Disabled!";
                                        AdminMessaging(m_Admin, m_MessageOut);
                                        GetGame().SetDebugMonitorEnabled(0);
                                        m_IsDebugRunning = false;
                                    }
                                    else
                                    {
                                        m_MessageOut = "Admin: " + m_AdminName + " DeBug Monitor (Status Monitor) Enabled!";
                                        AdminMessaging(m_Admin, m_MessageOut);
                                        GetGame().SetDebugMonitorEnabled(1);
                                        m_IsDebugRunning = true;
                                    }
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /spawncar
                                case "/spawncar":
                                    EntityAI MyV3S;
                                    vector NewPosition;
                                    vector OldPosition;
                                    OldPosition = m_Admin.GetPosition();
                                    NewPosition[0] = OldPosition[0] + 1.5;
                                    NewPosition[1] = OldPosition[1] + 0.2;
                                    NewPosition[2] = OldPosition[2] + 1.5;
                                    MyV3S = GetGame().CreateObject("OffroadHatchback", NewPosition, false, true, true);
                                    MyV3S.GetInventory().CreateAttachment("HatchbackHood");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackTrunk");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackDoors_CoDriver");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
                                    MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
                                    MyV3S.GetInventory().CreateAttachment("SparkPlug");
                                    MyV3S.GetInventory().CreateAttachment("EngineBelt");
                                    MyV3S.GetInventory().CreateAttachment("CarBattery");
                                    m_MessageOut = "Admin: " + m_AdminName + " has spawned in a vehicle";
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- /bandit
                                case "/bandit":
                                    m_Admin.RemoveAllItems();
                                    m_Admin.GetInventory().CreateInInventory("BallisticHelmet_Black");
                                    m_Admin.GetInventory().CreateInInventory("BagMountain_Green");
                                    m_Admin.GetInventory().CreateInInventory("CombatBoots_Brown");
                                    m_Admin.GetInventory().CreateInInventory("USMCPants_Woodland");
                                    m_Admin.GetInventory().CreateInInventory("PlateCarrierComplete2");
                                    m_Admin.GetInventory().CreateInInventory("SmershVestBackpack");
                                    m_Admin.GetInventory().CreateInInventory("TortillaBag");
                                    m_Admin.GetInventory().CreateInInventory("TacticalGloves_Green");
                                    m_Admin.GetInventory().CreateInInventory("TacticalShirt_Black");
                                    m_Admin.GetInventory().CreateInInventory("TacticalBaconCan_Opened");
                                    m_Admin.GetInventory().CreateInInventory("SpaghettiCan_Opened");
                                    m_Admin.GetInventory().CreateInInventory("SardinesCan_Opened");
                                    m_Admin.GetInventory().CreateInInventory("TunaCan_Opened");
                                    m_Admin.GetInventory().CreateInInventory("PersonalRadio");
                                    EntityAI rifles = m_Admin.GetHumanInventory().CreateInInventory("AKM");
                                    rifles.GetInventory().CreateAttachment("AK_RailHndgrd_Black");
                                    rifles.GetInventory().CreateAttachment("KobraOptic");
                                    rifles.GetInventory().CreateAttachment("AK_PlasticBttstck_Green");
                                    rifles.GetInventory().CreateAttachment("AK_Suppressor");
                                    addMags(m_Admin, "Mag_AKM_Drum75Rnd_Black", 3);
                                    m_Admin.LocalTakeEntityToHands(rifles);
                                    EntityAI entity_hands1 = m_Admin.GetHumanInventory().GetEntityInHands();
                                    m_Admin.SetQuickBarEntityShortcut(rifles, 0, true);
                                    m_MessageOut = "Admin: " + m_AdminName + " has spawned the bandit loadout";
                                    AdminMessaging(m_Admin, m_MessageOut);
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- !Invalid Command!
                                default:
                                    if (m_CmdParams[0].Substring(0, 1) == "/")
                                    {
                                        //Invalid Command
                                        m_MessageOut = "Invalid Command: " + m_CmdParams[0] + ".";
                                        AdminMessaging(m_Admin, m_MessageOut);
                                    }
                                break;
                                // -------------------------------------------------------------------------------------------------------------------------- END OF COMMANDS =================
                            }
                        }
                    }
                }
                break;
            //-------------- Vanilla----------
            case PreloadEventTypeID:
                PreloadEventParams preloadParams;
                Class.CastTo(preloadParams, params);
                OnPreloadEvent(preloadParams.param1, preloadParams.param2, preloadParams.param3, preloadParams.param4, preloadParams.param5);
            break;
            case ClientNewEventTypeID:
                ClientNewEventParams newParams;
                Class.CastTo(newParams, params);
                player = OnClientNewEvent(newParams.param1, newParams.param2, newParams.param3);
                if (!player)
                {
                    Debug.Log("ClientNewEvent: Player is empty");
                    return;
                }
                identity = newParams.param1;
                InvokeOnConnect(player,identity);
            break;
            case ClientReadyEventTypeID:
                ClientReadyEventParams readyParams;
                Class.CastTo(readyParams, params);
                identity = readyParams.param1;
                Class.CastTo(player, readyParams.param2);
                if (!player)
                {
                    Debug.Log("ClientReadyEvent: Player is empty");
                    return;
                }
                OnClientReadyEvent(identity, player);
                InvokeOnConnect(player, identity);
            break;
            case ClientRespawnEventTypeID:
                ClientRespawnEventParams respawnParams;
                Class.CastTo(respawnParams, params);
                identity = respawnParams.param1;
                Class.CastTo(player, respawnParams.param2);
                if (!player)
                {
                    Debug.Log("ClientRespawnEvent: Player is empty");
                    return;
                }
                OnClientRespawnEvent(identity, player);
            break;
            case ClientDisconnectedEventTypeID:
                ClientDisconnectedEventParams discoParams;
                Class.CastTo(discoParams, params);
                identity = discoParams.param1;
                Class.CastTo(player, discoParams.param2);
                int discTime = discoParams.param3;
                bool authFailed = discoParams.param4;
                if (!player)
                {
                    Debug.Log("ClientDisconnectenEvent: Player is empty");
                    return;
                }
                InvokeOnDisconnect(player);
                OnClientDisconnectedEvent(identity, player, discTime, authFailed);
            break;
            case LogoutCancelEventTypeID:
                LogoutCancelEventParams logoutCancelParams;
                Class.CastTo(logoutCancelParams, params);
                Class.CastTo(player, logoutCancelParams.param1);
                identity = player.GetIdentity();
                if (identity)
                {
                    Print("[Logout]: Player " + identity.GetId() + " cancelled");
                }
                else
                {
                    Print("[Logout]: Player cancelled"); 
                }
                m_LogoutPlayers.Remove(player);
            break;
        }
    }
	
	void TempGodMode()
	{
		for ( int tg = 0; tg < m_Admins.Count(); ++tg )
		{
			if(m_Admins.Get(tg) == NULL) m_Admins.Remove(tg);
			PlayerBase player = m_Admins.Get(tg);
			if( player.IsAlive() && player )
			{
				string PlayerUID = player.GetIdentity().GetId();
				g_Game.GetProfileString("adminGodMode"+PlayerUID, godMode);
				if(debugTempGodMode) PrintFormat( "[DonkeyAHAT] - init.c TempGodMode godMode:%1 m_Admins:%2", godMode, m_Admins );
				if ( godMode == "true" )
				{
					player.SetHealth( player.GetMaxHealth( "", "" ) );
					player.SetHealth( "","Blood", player.GetMaxHealth( "", "Blood" ) );
					player.SetHealth("","Shock", player.GetMaxHealth("","Shock") );
					
					float energy = player.GetStatEnergy().Get();
					player.GetStatEnergy().Add( energy + 1.25 );
					
					float water = player.GetStatWater().Get();
					player.GetStatWater().Add( water + 1.25 );
					
					player.GetStatStamina().Set(1000); // Not sure if working or not
					
					player.GetStatWet().Add( -1 );
					
					
					//player.GetStatTemperature().Add( (TEMPERATURE_INCREMENT_PER_SEC*deltaT) );
					//TREMOR_TRESHOLD = 35.5
					//player.GetStatTremor().Set( TREMOR_TRESHOLD - player.GetStatTemperature().Get() );
					//Make a player tremor, set their temperature to lower than 35.5
					
					if(debugTempGodMode) PrintFormat( "[DonkeyAHAT] - init.c TempGodMode energy:%1 water:%2", energy, water );
										
					EntityAI CurrentWeapon = player.GetHumanInventory().GetEntityInHands();
					if( CurrentWeapon )
					{
						Magazine foundMag = ( Magazine ) CurrentWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );
						if( foundMag && foundMag.IsMagazine())
						{
							foundMag.ServerSetAmmoMax();
						}

						Object Suppressor = ( Object ) CurrentWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );
						if( Suppressor )
						{
							Suppressor.SetHealth( Suppressor.GetMaxHealth( "", "" ) );
						}
					}
				}
			}else{
				m_Admins.Remove(tg);
			}
		}
	}