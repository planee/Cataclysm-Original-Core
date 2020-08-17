#ifndef DEF_GRIMBATOL_H
#define DEF_GRIMBATOL_H

const Position batteryDragonFlyPath[19] =
{
    { -477.33f, -351.18f, 302.77f, 2.74f },
    { -561.09f, -338.10f, 311.42f, 3.40f },
    { -635.61f, -371.63f, 315.52f, 3.77f },
    { -718.27f, -456.83f, 325.32f, 4.09f },
    { -710.35f, -580.67f, 320.62f, 4.80f },
    { -670.26f, -656.58f, 303.07f, 5.40f },
    { -579.76f, -715.76f, 301.51f, 5.74f },
    { -482.80f, -712.38f, 315.92f, 0.11f },
    { -402.45f, -678.40f, 311.11f, 0.44f },
    { -482.80f, -712.38f, 315.92f, 0.11f },
    { -579.76f, -715.76f, 301.51f, 5.74f },
    { -670.26f, -656.58f, 303.07f, 5.40f },
    { -710.35f, -580.67f, 320.62f, 4.80f },
    { -718.27f, -456.83f, 325.32f, 4.09f },
    { -635.61f, -371.63f, 315.52f, 3.77f },
    { -561.09f, -338.10f, 311.42f, 3.40f },
    { -477.33f, -351.18f, 302.77f, 2.74f },
};

const Position  twilightarcherPos[13] =
{
    { -542.994f, -605.236f, 300.201f, 1.68049f },
    { -543.59f, -605.413f, 283.784f,  1.50377f },
    { -521.237f, -605.435f, 300.76f,  1.63886f },
    { -483.862f, -588.658f, 297.574f, 2.38106f },
    { -482.655f, -588.461f, 280.966f, 2.34571f },
    { -471.266f, -575.324f, 295.906f, 2.30254f },
    { -525.377f, -455.312f, 285.288f, 4.66187f },
    { -544.49f, -454.961f, 295.831f,  4.79539f },
    { -522.164f, -455.31f, 299.791f,  4.77575f },
    { -468.703f, -489.004f, 300.462f, 3.78616f },
    { -470.907f, -484.791f, 282.203f, 3.87255f },
    { -485.052f, -474.621f, 296.525f, 3.92361f },
    { -481.352f, -477.21f, 280.714f,  3.72334f },
};

const Position drahgavalionaPos[3] =
{
    { -373.35f, -659.56f, 253.56f, 3.44f },
    { -373.35f, -659.56f, 282.26f, 3.44f },
    { -414.04f, -681.35f, 267.09f, 3.63f },
};

const Position neareggPos[2] =
{
    { -719.67f, -839.64f, 232.43f, 0.0f    },
    //{ -731.72f, -787.40f, 232.47f, 2.25f },
    { -730.63f, -864.46f, 232.44f, 4.71f   },
};
const Position shadowgalePos[3] =
{
    { -745.07f, -845.16f, 232.41f, 0.0f },
    { -724.05f, -823.47f, 232.41f, 0.0f },
    { -741.81f, -819.44f, 232.41f, 0.0f },
};
const Position eggPos[23] =
{
    { -720.21f, -875.28f, 232.56f, 4.59f },
    { -728.52f, -875.78f, 232.55f, 3.33f },
    { -736.79f, -877.43f, 232.58f, 3.33f },
    { -748.38f, -876.34f, 233.23f, 2.87f },
    { -757.47f, -873.80f, 233.23f, 2.87f },
    { -749.23f, -871.13f, 233.23f, 0.30f },
    { -740.28f, -870.06f, 232.58f, 6.17f },
    { -731.70f, -868.19f, 232.47f, 0.00f },
    { -752.09f, -860.58f, 232.54f, 3.13f },
    { -747.30f, -779.60f, 233.23f, 1.92f },
    { -740.44f, -778.07f, 233.23f, 0.22f },
    { -729.57f, -778.52f, 232.57f, 0.11f },
    { -719.22f, -770.13f, 233.44f, 0.72f },
    { -722.09f, -780.99f, 232.55f, 4.45f },
    { -735.99f, -786.08f, 232.57f, 3.05f },
    { -751.92f, -788.38f, 233.23f, 3.33f },
    { -745.56f, -785.41f, 233.23f, 0.43f },
    { -757.08f, -780.37f, 233.23f, 2.68f },
    { -746.64f, -770.96f, 233.23f, 0.95f },
    { -731.53f, -770.02f, 236.14f, 0.10f },
    { -730.15f, -885.09f, 235.96f, 5.93f },
    { -717.82f, -887.81f, 233.95f, 6.05f },
    { -751.01f, -886.38f, 234.87f, 3.27f },
};

const Position eggsSpawnPath[2] =
{
    { -729.98f, -784.89f, 232.50f, 1.77f },
    { -728.31f, -867.80f, 232.47f, 5.16f },
};

const Position DrahgaLeavePoint  = { -396.02f, -674.77f, 265.14f, 0.35f };
const Position ValioneLeavePoint = { -373.56f, -613.11f, 299.54f, 0.93f };
const Position erudaxportalPos   = { -641.515f, -827.8f, 235.5f, 3.069f };

enum Npcs
{
    NPC_GENERAL_UMBRISS              = 39625,
    NPC_FORGEMASTER_THRONGUS         = 40177,
    NPC_DRAHGA_SHADOWBURNER          = 40319,
    NPC_ERUDAX                       = 40484,
};

enum Data
{
    DATA_GENERAL_UMBRISS        = 0,
    DATA_FORGEMASTER_THRONGUS   = 1,
    DATA_DRAHGA_SHADOWBURNER    = 2,
    DATA_ERUDAX                 = 3,

    // Misc
    DATA_BREAK_THE_EGGS,
};

enum Types
{
    TYPE_WEAPON_DATA,
};

enum Achievements
{
    ACHIEV_UMBRAGE_FOR_UMBRISS      = 5297,
};

template<class AI>
CreatureAI* GetInstanceAI(Creature* creature)
{
    if (InstanceMap* instance = creature->GetMap()->ToInstanceMap())
        if (instance->GetInstanceScript())
            if (instance->GetScriptId() == sObjectMgr->GetScriptId("instance_grim_batol"))
                return new AI(creature);
    return NULL;
}

#endif
