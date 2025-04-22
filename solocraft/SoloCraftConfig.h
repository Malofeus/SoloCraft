#pragma once

#include "Config/Config.h"

#include <unordered_map>

class SolocraftConfig
{
    public:
        SolocraftConfig();
 
        bool Initialize();

        bool enabled;
        bool SoloCraftAnnounceModule;
        bool SoloCraftDebuffEnable;
        bool SolocraftXPBalEnabled;
        bool SolocraftXPEnabled;
        bool SolocraftNoXPFlag;
        float SoloCraftSpellMult;
        float SoloCraftStatsMult;
        float SoloCraftXPMod;
        uint32 SolocraftLevelDiff;
        uint32 SolocraftDungeonLevel;
        std::unordered_map<uint8, uint32> classes;
        std::unordered_map<uint32, uint32> dungeons;
        std::unordered_map<uint32, float> diff_Multiplier;
        std::unordered_map<uint32, float> diff_Multiplier_Heroics;
        float D5 = 1.0;
        float D10 = 1.0;
        float D25 = 1.0;
        float D40 = 1.0;
        float D649H10 = 1.0;
        float D649H25 = 1.0;

    private:
        Config config;
};

// #define sSolocraftConfig MaNGOS::Singleton<SolocraftConfig>::Instance()