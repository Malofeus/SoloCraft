#pragma once

#include "solocraft/SoloCraftConfig.h"

#include "Globals/ObjectMgr.h"
#include "Entities/ObjectGuid.h"
#include "Entities/Player.h"
#include "Maps/Map.h"

#include <map>

class Solocraft
{
    public:
        Solocraft();
        virtual ~Solocraft();
        static Solocraft& instance()
        {
            static Solocraft instance;
            return instance;
        }

        bool Initialize();
        void OnLogin(Player* player);
        void OnLogout(Player* player);
        //void OnAddMember(Player* player);
        void OnMapChanged(Player* player);
        //bool ChatFilter(Player* player, string text);

    private:
        SolocraftConfig sSolocraftConfig;
	    std::map<ObjectGuid, float> _unitDifficulty;
        std::map<ObjectGuid, float> _unitBuff;
        
        uint32 CalculateDungeonlevel(Map* map);
        float CalculateDifficulty(Map* map);
        uint32 GetNumInGroup(Player* payer);
        uint32 GetClassBalance(Player* player);
        void ClearBuffs(Player* player, Map* map);
        void ClearBuffs(Player* player);
        void ApplyBuffs(Player* player, Map* map, uint32 dunLevel, float difficulty, uint32 numInGroup, uint32 classBalance);
        void ApplyBuffs(Player* player, uint32 multiplier);
};

#define sSolocraft MaNGOS::Singleton<Solocraft>::Instance()