#include "WorldPacket.h"
#include "MiscPackets.h"

class StormsReachMapScript : public WorldMapScript
{
public:
    StormsReachMapScript() : WorldMapScript("storms_reach_map_script", 2000)
    {
    }

    void OnCreate(Map* map) override
    {
        mapCurrentTime = GameTime::GetGameTime();
        TC_LOG_INFO("server.loading", "Storms Reach : Map Created");
    }

    void OnPlayerEnter(Map* map, Player* player) override
    {
        // Nightfall: Change in game speed to 2 hours per day-night cycle (4 hour day)
        WorldPackets::Misc::LoginSetTimeSpeed loginSetTimeSpeed;
        loginSetTimeSpeed.NewSpeed = 0.10000002f;
        loginSetTimeSpeed.GameTime = mapCurrentTime;
        loginSetTimeSpeed.ServerTime = GameTime::GetGameTime();

        player->SendDirectMessage(loginSetTimeSpeed.Write());
        TC_LOG_INFO("server.loading", "Storms Reach : Player Entered");
    }

    void OnPlayerLeave(Map* map, Player* player) override
    {
        // Reset time progression back to normal time for players leaving the map
        WorldPackets::Misc::LoginSetTimeSpeed loginSetTimeSpeed;
        loginSetTimeSpeed.NewSpeed = 0.01666667f;
        loginSetTimeSpeed.GameTime = GameTime::GetGameTime();
        loginSetTimeSpeed.ServerTime = GameTime::GetGameTime();

        player->SendDirectMessage(loginSetTimeSpeed.Write());
        TC_LOG_INFO("server.loading", "Storms Reach : Player Exited");
    }

    void OnUpdate(Map* map, uint32 diff) override
    {

        // Update the current time every 10 seconds (6x faster time = 10 seconds per in game minute)
        //mapCurrentTime += static_cast<time_t>(std::floor((diff / 1000.0f) * 6));
        //TC_LOG_INFO("server.loading", "Storms Reach : Player Exited - Time added: %u seconds", static_cast<uint32>(std::floor((diff / 1000.0f) * 6)));
        TC_LOG_INFO("server.loading", "ONUPDATE");
        
        // Make sure we only tick 1 per second for core logic
      /*  _coreLoopTime += diff;
        if (_coreLoopTime < 1000)
            return;
        _coreLoopTime = 0;*/

        // Core Logic
        //TC_LOG_INFO("server.loading", "Storms Reach : Player Exited");
    }

private:
    uint32 _coreLoopTime = 0; 
    uint32 _timeLoopDelta = 0;
    time_t mapCurrentTime;
};

void NightfallMapScripts()
{
    new StormsReachMapScript();
}