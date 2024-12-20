/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
Name: honor_commandscript
%Complete: 100
Comment: All honor related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"
#include "WorldPacket.h"
#include "MiscPackets.h"
#include "Player.h"
#include "TaskScheduler.h"

class nightfall_commandscript : public CommandScript
{
public:
    nightfall_commandscript() : CommandScript("nightfall_commandscript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> honorAddCommandTable =
        {
            { "kill",           SEC_GAMEMASTER,     false, &HandleHonorAddKillCommand,         ""},
            { "",               SEC_GAMEMASTER,     false, &HandleHonorAddCommand,             ""}
        };

        static std::vector<ChatCommand> honorCommandTable =
        {
            { "add",            SEC_GAMEMASTER,     false, NULL,               "", honorAddCommandTable },
            { "trigger",         SEC_GAMEMASTER,     false, &HandleNightfallTrigger,          ""}
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "nightfall",          SEC_GAMEMASTER,     false, NULL,                  "", honorCommandTable }
        };
        return commandTable;
    }

    static bool HandleHonorAddCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if (handler->HasLowerSecurity(target, ObjectGuid::Empty))
            return false;

        uint32 amount = (uint32)atoi(args);
        target->RewardHonor(NULL, 1, amount);
        return true;
    }

    static bool HandleHonorAddKillCommand(ChatHandler* handler, char const* /*args*/)
    {
        Unit* target = handler->getSelectedUnit();
        if (!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if (target->GetTypeId() == TYPEID_PLAYER && handler->HasLowerSecurity((Player*)target, ObjectGuid::Empty))
            return false;

        handler->GetSession()->GetPlayer()->RewardHonor(target, 1);
        return true;
    }

    static bool HandleNightfallTrigger(ChatHandler* handler, char const* /*args*/)
    {
        Player* target = handler->getSelectedPlayer();
        if (!target)
        {
            handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
            handler->SetSentErrorMessage(true);
            return false;
        }

        // check online security
        if (handler->HasLowerSecurity(target, ObjectGuid::Empty))
            return false;


        // Stuff
        struct tm midnightTime = {};
        midnightTime.tm_hour = 0;   // 0 represents midnight
        midnightTime.tm_min = 0;
        midnightTime.tm_sec = 0;
        midnightTime.tm_isdst = -1; // Let the system determine daylight saving time

        // Convert to time_t
        time_t midnightTimestamp = mktime(&midnightTime);

        WorldPackets::Misc::LoginSetTimeSpeed loginSetTimeSpeed;
        loginSetTimeSpeed.NewSpeed = 20.0f;
        loginSetTimeSpeed.GameTime = midnightTimestamp;
        loginSetTimeSpeed.ServerTime = GameTime::GetGameTime();
        handler->GetSession()->GetPlayer()->SendDirectMessage(loginSetTimeSpeed.Write());

        // Time out here
      

        return true;
    }
};

void AddSC_nightfall_commandscript()
{
    new nightfall_commandscript();
}
