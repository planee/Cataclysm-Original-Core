#include "JamClient.h"

/*static*/ void JamClient::CalculateOpcode(int p_Opcode, std::vector<std::string>& p_Func)
{
    int a4 = p_Opcode;

    int v5; // esi

    v5 = a4 & 0x10;
    switch (a4 & 7 | ((v5 | ((a4 & 0x100 | ((a4 & 0xC00 | ((unsigned int)a4 >> 1) & 0x3000) >> 1)) >> 3)) >> 1))
    {
    case 1u:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v108 = p_Func.push_back("sub_6B8360(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v108;
        break;
    case 4u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v132 = p_Func.push_back("sub_6A78B0(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v132;
        break;
    case 5u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v245 = p_Func.push_back("sub_6BF450(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v245;
        break;
    case 7u:
        p_Func.push_back("sub_6F2290(&//v269, (int)this, a2, (int)a5, a3);");
        //v259 = p_Func.push_back("sub_6B1CF0(&//v269);");
        p_Func.push_back("sub_6E20D0(&//v269);");
        //result = //v259;
        break;
    case 8u:
        p_Func.push_back("sub_6F4330(&//v294, (int)this, a2, (unsigned int)a5, a3);");
        //v26 = p_Func.push_back("sub_6B5040(&//v294);");
        p_Func.push_back("sub_6F0070(&//v294);");
        //result = //v26;
        break;
    case 9u:
        p_Func.push_back("sub_6B12A0(&//v292, (int)this, a2, a5, a3);");
        //v116 = p_Func.push_back("sub_6A9040(&//v292);");
        p_Func.push_back("sub_6A1E20(&//v292);");
        //result = //v116;
        break;
    case 0xAu:
        p_Func.push_back("sub_6E9330(&//v299, (int)this, a2, a5, a3);");
        //v243 = p_Func.push_back("sub_6BFD00(&//v299);");
        p_Func.push_back("sub_6E57C0(&//v299);");
        //result = //v243;
        break;
    case 0xBu:
        p_Func.push_back("sub_6EE9F0(&//v299, (int)this, a2, a5, a3);");
        //v34 = p_Func.push_back("sub_6A3300(&//v299);");
        p_Func.push_back("sub_6E5530(&//v299);");
        //result = //v34;
        break;
    case 0xEu:
        p_Func.push_back("sub_6E9370(&//v299, (int)this, a2, (int)a5, a3);");
        //v36 = p_Func.push_back("sub_6A72A0(&//v299);");
        p_Func.push_back("sub_6E53C0(&//v299);");
        //result = //v36;
        break;
    case 0x13u:
        p_Func.push_back("sub_6F4270(&//v299, (int)this, a2, a5, a3);");
        //v218 = p_Func.push_back("sub_6BA140(&//v299);");
        p_Func.push_back("sub_6E9300(&//v299);");
        //result = //v218;
        break;
    case 0x17u:
        p_Func.push_back("sub_6C54C0((int)&//v299, (int)this, a2, a5, a3);");
        //v185 = p_Func.push_back("sub_6C0D40(&//v299);");
        p_Func.push_back("sub_6A2ED0(&//v299);");
        //result = //v185;
        break;
    case 0x18u:
        p_Func.push_back("sub_6C0E20(&//v297, (int)this, a2, (int)a5, a3);");
        //v186 = p_Func.push_back("sub_6A5BB0(&//v297);");
        p_Func.push_back("sub_6A1690(&//v297);");
        //result = //v186;
        break;
    case 0x19u:
        p_Func.push_back("sub_6EF050(&//v299, (int)this, a2, (int)a5, a3);");
        //v256 = p_Func.push_back("sub_6B81A0(&//v299);");
        p_Func.push_back("sub_6E02A0(&//v299);");
        //result = //v256;
        break;
    case 0x1Au:
        p_Func.push_back("sub_6D97F0(&//v278, (int)this, a2, (int)a5, a3);");
        //v213 = p_Func.push_back("sub_6ADB90(&//v278);");
        p_Func.push_back("sub_6A21C0(&//v278);");
        //result = //v213;
        break;
    case 0x1Bu:
        p_Func.push_back("sub_6C8800((int)&//v301, (int)this, a2, a5, a3);");
        //v135 = p_Func.push_back("sub_6B2A30(&//v301);");
        p_Func.push_back("sub_6A20D0(&//v301);");
        //result = //v135;
        break;
    case 0x1Cu:
        p_Func.push_back("sub_6A12D0(&//v289, (int)this, a2, a5, a3);");
        //v69 = p_Func.push_back("sub_6BB740(&//v289);");
        p_Func.push_back("sub_6A0F00(&//v289);");
        //result = //v69;
        break;
    case 0x1Du:
        p_Func.push_back("sub_6AEF10(&//v301, (int)this, a2, a5, a3);");
        //v128 = p_Func.push_back("sub_6AD140(&//v301);");
        p_Func.push_back("sub_6B1AE0(&//v301);");
        //result = //v128;
        break;
    case 0x20u:
        p_Func.push_back("sub_6D5A00(&//v268, (int)this, a2, a5, a3);");
        //v234 = p_Func.push_back("sub_6A9870(&//v268);");
        p_Func.push_back("sub_6A0DF0(&//v268);");
        //result = //v234;
        break;
    case 0x21u:
        p_Func.push_back("sub_6B3230(&//v301, (int)this, a2, a5, a3);");
        //v86 = p_Func.push_back("sub_6BC8A0(&//v301);");
        p_Func.push_back("sub_6A2650(&//v301);");
        //result = //v86;
        break;
    case 0x22u:
        p_Func.push_back("sub_6AC0A0(&//v277, (int)this, a2, a5, a3);");
        //v90 = p_Func.push_back("sub_6BEEB0(&//v277);");
        p_Func.push_back("sub_6A1F50(&//v277);");
        //result = //v90;
        break;
    case 0x23u:
        p_Func.push_back("sub_6BE8D0((int)&//v299, (int)this, a2, a5, a3);");
        //v223 = p_Func.push_back("sub_6BF780(&//v299);");
        p_Func.push_back("sub_6A2EB0(&//v299);");
        //result = //v223;
        break;
    case 0x26u:
        p_Func.push_back("sub_6D72B0(&//v282, (int)this, a2, a5, a3);");
        //v137 = p_Func.push_back("sub_6AC050(&//v282);");
        p_Func.push_back("sub_6A22D0(&//v282);");
        //result = //v137;
        break;
    case 0x28u:
        p_Func.push_back("sub_6B0860((int)&//v299, (int)this, a2, a5, a3);");
        //v158 = p_Func.push_back("sub_6AD0C0(&//v299);");
        p_Func.push_back("sub_6A1280(&//v299);");
        //result = //v158;
        break;
    case 0x29u:
        p_Func.push_back("sub_6AA2E0(&//v301, (int)this, a2, a5, a3);");
        //v115 = p_Func.push_back("sub_6BE000(&//v301);");
        p_Func.push_back("sub_6A1CD0(&//v301);");
        //result = //v115;
        break;
    case 0x2Cu:
        p_Func.push_back("sub_6ECE10(&//v299, (int)this, a2, a5, a3);");
        //v78 = p_Func.push_back("sub_6A8D90(&//v299);");
        p_Func.push_back("sub_6E2C90(&//v299);");
        //result = //v78;
        break;
    case 0x2Eu:
        p_Func.push_back("sub_6C17F0((int)&//v301, (int)this, a2, a5, a3);");
        //v198 = p_Func.push_back("sub_6AAF20(&//v301);");
        p_Func.push_back("sub_6A25D0(&//v301);");
        //result = //v198;
        break;
    case 0x2Fu:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v88 = p_Func.push_back("sub_6A6E00(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v88;
        break;
    case 0x31u:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v103 = p_Func.push_back("sub_6A8EB0(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v103;
        break;
    case 0x32u:
        p_Func.push_back("sub_6C4950((int)&//v299, (int)this, a2, a5, a3);");
        //v179 = p_Func.push_back("sub_6B1A20(&//v299);");
        p_Func.push_back("sub_6A23D0(&//v299);");
        //result = //v179;
        break;
    case 0x33u:
        p_Func.push_back("sub_6D6510((int)&//v284, (int)this, a2, a5, a3);");
        //v94 = p_Func.push_back("sub_6ABBF0(&//v284);");
        p_Func.push_back("sub_6CA830(&//v284);");
        //result = //v94;
        break;
    case 0x34u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v257 = p_Func.push_back("sub_6B2230(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v257;
        break;
    case 0x35u:
        p_Func.push_back("sub_6F2DB0(&//v299, (unsigned int)this, a2, a5, a3);");
        //v13 = p_Func.push_back("sub_6B0440(&//v299);");
        p_Func.push_back("sub_6DFCD0(&//v299);");
        //result = //v13;
        break;
    case 0x36u:
        p_Func.push_back("sub_6C3150(&//v293, (int)this, a2, a5, a3);");
        //v61 = p_Func.push_back("sub_6B83A0(&//v293);");
        p_Func.push_back("sub_6A2CD0(&//v293);");
        //result = //v61;
        break;
    case 0x37u:
        p_Func.push_back("sub_6C46D0((int)&//v299, (int)this, a2, a5, a3);");
        //v161 = p_Func.push_back("sub_6A6690(&//v299);");
        p_Func.push_back("sub_6A28C0(&//v299);");
        //result = //v161;
        break;
    case 0x38u:
        p_Func.push_back("sub_6EE9A0(&//v293, (int)this, a2, a5, a3);");
        //v11 = p_Func.push_back("sub_6A55C0(&//v293);");
        p_Func.push_back("sub_6E59D0(&//v293);");
        //result = //v11;
        break;
    case 0x39u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v39 = p_Func.push_back("sub_6AA4A0(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v39;
        break;
    case 0x3Fu:
        p_Func.push_back("sub_6DA0E0(&//v299, (int)this, a2, a5, a3);");
        //v110 = p_Func.push_back("sub_6B4890(&//v299);");
        p_Func.push_back("sub_6DBA60(&//v299);");
        //result = //v110;
        break;
    case 0x46u:
        p_Func.push_back("sub_6CCA40((int)&//v297, (int)this, a2, a5, a3);");
        //v238 = p_Func.push_back("sub_6B2270(&//v297);");
        p_Func.push_back("sub_6D5A70(&//v297);");
        //result = //v238;
        break;
    case 0x47u:
        p_Func.push_back("sub_6A3270(&//v302, (int)this, a2, a5, a3);");
        //v226 = p_Func.push_back("sub_6AD480(&//v302);");
        p_Func.push_back("sub_6A19B0(&//v302);");
        //result = //v226;
        break;
    case 0x48u:
        p_Func.push_back("sub_6F0820(&//v299, (int)this, a2, (int)a5, a3);");
        //v30 = p_Func.push_back("sub_6A5B20(&//v299);");
        p_Func.push_back("sub_6D59C0(&//v299);");
        //result = //v30;
        break;
    case 0x49u:
        p_Func.push_back("sub_6A2B20(&//v287, (int)this, a2, a5, a3);");
        //v130 = p_Func.push_back("sub_6ADB30(&//v287);");
        p_Func.push_back("sub_6A1FD0(&//v287);");
        //result = //v130;
        break;
    case 0x4Eu:
        p_Func.push_back("sub_6E2C50(&//v299, (int)this, a2, a5, a3);");
        //v45 = p_Func.push_back("sub_6A5650(&//v299);");
        p_Func.push_back("sub_6DB2D0(&//v299);");
        //result = //v45;
        break;
    case 0x53u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v214 = p_Func.push_back("sub_6BC5C0(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v214;
        break;
    case 0x54u:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v236 = p_Func.push_back("sub_6B0AE0(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v236;
        break;
    case 0x55u:
        p_Func.push_back("sub_6D0D30((int)&//v297, (int)this, a2, a5, a3);");
        //v134 = p_Func.push_back("sub_6A4190(&//v297);");
        p_Func.push_back("sub_6A1100(&//v297);");
        //result = //v134;
        break;
    case 0x5Bu:
        p_Func.push_back("sub_6AC7E0((int)&//v299, (int)this, a2, a5, a3);");
        //v189 = p_Func.push_back("sub_6A7250(&//v299);");
        p_Func.push_back("sub_6A1DA0(&//v299);");
        //result = //v189;
        break;
    case 0x5Du:
        p_Func.push_back("sub_6E1E10(&//v299, (int)this, a2, (int)a5, a3);");
        //v72 = p_Func.push_back("sub_6C09F0(&//v299);");
        p_Func.push_back("sub_6DDEB0(&//v299);");
        //result = //v72;
        break;
    case 0x60u:
        p_Func.push_back("sub_6B1EA0((int)&//v301, (int)this, a2, a5, a3);");
        //v95 = p_Func.push_back("sub_6BE2C0(&//v301);");
        p_Func.push_back("sub_6A1230(&//v301);");
        //result = //v95;
        break;
    case 0x65u:
        p_Func.push_back("sub_6EF880(&//v298, (int)this, a2, (unsigned int)a5, a3);");
        //v121 = p_Func.push_back("sub_6AA9F0(&//v298);");
        p_Func.push_back("sub_6DAF00(&//v298);");
        //result = //v121;
        break;
    case 0x68u:
        p_Func.push_back("sub_6C7880((int)&//v275, (int)this, a2, a5, a3);");
        //v33 = p_Func.push_back("sub_6BE7F0(&//v275);");
        p_Func.push_back("sub_6A1980(&//v275);");
        //result = //v33;
        break;
    case 0x6Au:
        p_Func.push_back("sub_6F3670(&//v298, (int)this, a2, (unsigned int)a5, a3);");
        //v119 = p_Func.push_back("sub_6BE0D0(&//v298);");
        p_Func.push_back("sub_6E76A0(&//v298);");
        //result = //v119;
        break;
    case 0x6Du:
        p_Func.push_back("sub_6BF330((int)&//v301, (int)this, a2, a5, a3);");
        //v210 = p_Func.push_back("sub_6A3230(&//v301);");
        p_Func.push_back("sub_6A1140(&//v301);");
        //result = //v210;
        break;
    case 0x6Fu:
        p_Func.push_back("sub_6DC660(&//v297, (int)this, a2, a5, a3);");
        //v35 = p_Func.push_back("sub_6C0CC0(&//v297);");
        p_Func.push_back("sub_6D7310(&//v297);");
        //result = //v35;
        break;
    case 0x72u:
        p_Func.push_back("sub_6C48C0((int)&//v301, (int)this, a2, a5, a3);");
        //v203 = p_Func.push_back("sub_6B1AA0(&//v301);");
        p_Func.push_back("sub_6A0E20(&//v301);");
        //result = //v203;
        break;
    case 0x73u:
        p_Func.push_back("sub_6F07A0(&//v299, (int)this, a2, (unsigned int)a5, a3);");
        //v118 = p_Func.push_back("sub_6AD100(&//v299);");
        p_Func.push_back("sub_6E57F0(&//v299);");
        //result = //v118;
        break;
    case 0x74u:
        p_Func.push_back("sub_6AA530((int)&//v299, (int)this, a2, a5, a3);");
        //v165 = p_Func.push_back("sub_6BD9D0(&//v299);");
        p_Func.push_back("sub_6A2A10(&//v299);");
        //result = //v165;
        break;
    case 0x76u:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v229 = p_Func.push_back("sub_6A3CC0(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v229;
        break;
    case 0x77u:
        p_Func.push_back("sub_6C0D00((int)&//v297, (int)this, a2, a5, a3);");
        //v222 = p_Func.push_back("sub_6C0800(&//v297);");
        p_Func.push_back("sub_6A2D90(&//v297);");
        //result = //v222;
        break;
    case 0x78u:
        p_Func.push_back("sub_6E9810(&//v298, (int)this, a2, (int)a5, a3);");
        //v43 = p_Func.push_back("sub_6C0180(&//v298);");
        p_Func.push_back("sub_6E1FC0(&//v298);");
        //result = //v43;
        break;
    case 0x7Cu:
        p_Func.push_back("sub_6A3270(&//v302, (int)this, a2, a5, a3);");
        //v31 = p_Func.push_back("sub_6B0650(&//v302);");
        p_Func.push_back("sub_6A19B0(&//v302);");
        //result = //v31;
        break;
    case 0x7Du:
        p_Func.push_back("sub_6CB470(&//v282, (int)this, a2, a5, a3);");
        //v145 = p_Func.push_back("sub_6B6410(&//v282);");
        p_Func.push_back("sub_6A30C0(&//v282);");
        //result = //v145;
        break;
    case 0x7Eu:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v237 = p_Func.push_back("sub_6A7500(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v237;
        break;
    case 0x80u:
        p_Func.push_back("sub_6A9430(&//v300, (int)this, a2, a5, a3);");
        //v32 = p_Func.push_back("sub_6B06E0(&//v300);");
        p_Func.push_back("sub_6A0D70(&//v300);");
        //result = //v32;
        break;
    case 0x81u:
        p_Func.push_back("sub_6BEDF0(&//v294, (int)this, a2, a5, a3);");
        //v18 = p_Func.push_back("sub_6B05D0(&//v294);");
        p_Func.push_back("sub_6A7330(&//v294);");
        //result = //v18;
        break;
    case 0x82u:
        p_Func.push_back("sub_6C6030((int)&//v299, (int)this, a2, a5, a3);");
        //v164 = p_Func.push_back("sub_6B1A60(&//v299);");
        p_Func.push_back("sub_6A2380(&//v299);");
        //result = //v164;
        break;
    case 0x85u:
        p_Func.push_back("sub_6AC0E0((int)&//v299, (int)this, a2, a5, a3);");
        //v166 = p_Func.push_back("sub_6B2E50(&//v299);");
        p_Func.push_back("sub_6A10F0(&//v299);");
        //result = //v166;
        break;
    case 0x86u:
        p_Func.push_back("sub_6BADE0((int)&//v299, (int)this, a2, a5, a3);");
        //v211 = p_Func.push_back("sub_6B45A0(&//v299);");
        p_Func.push_back("sub_6A2D70(&//v299);");
        //result = //v211;
        break;
    case 0x87u:
        p_Func.push_back("sub_6DD590(&//v298, (int)this, a2, a5, a3);");
        //v66 = p_Func.push_back("sub_6C0DA0(&//v298);");
        p_Func.push_back("sub_6CAD20(&//v298);");
        //result = //v66;
        break;
    case 0x89u:
        p_Func.push_back("sub_6D8670(&//v282, (int)this, a2, a5, a3);");
        //v142 = p_Func.push_back("sub_6B8950(&//v282);");
        p_Func.push_back("sub_6A2520(&//v282);");
        //result = //v142;
        break;
    case 0x8Au:
        p_Func.push_back("sub_6EAF10(&//v293, (int)this, a2, a5, a3);");
        //v240 = p_Func.push_back("sub_6AA4E0(&//v293);");
        p_Func.push_back("sub_6D9740(&//v293);");
        //result = //v240;
        break;
    case 0x8Bu:
        p_Func.push_back("sub_6B9430((int)&//v299, (int)this, a2, a5, a3);");
        //v183 = p_Func.push_back("sub_6A8B20(&//v299);");
        p_Func.push_back("sub_6A0F20(&//v299);");
        //result = //v183;
        break;
    case 0x8Cu:
        p_Func.push_back("sub_6F42F0(&//v299, (int)this, a2, (int)a5, a3);");
        //v85 = p_Func.push_back("sub_6B2870(&//v299);");
        p_Func.push_back("sub_6D92C0(&//v299);");
        //result = //v85;
        break;
    case 0x8Du:
        p_Func.push_back("sub_6D4580(&//v282, (int)this, a2, (int)a5, a3);");
        //v141 = p_Func.push_back("sub_6B2E90(&//v282);");
        p_Func.push_back("sub_6A10A0(&//v282);");
        //result = //v141;
        break;
    case 0x8Eu:
        p_Func.push_back("sub_6C4910(&//v302, (int)this, a2, a5, a3);");
        //v232 = p_Func.push_back("sub_6B3C50(&//v302);");
        p_Func.push_back("sub_6A1F40(&//v302);");
        //result = //v232;
        break;
    case 0x8Fu:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v105 = p_Func.push_back("sub_6B3B10(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v105;
        break;
    case 0x90u:
        p_Func.push_back("sub_6BEDF0(&//v294, (int)this, a2, a5, a3);");
        //v19 = p_Func.push_back("sub_6C0960(&//v294);");
        p_Func.push_back("sub_6A7330(&//v294);");
        //result = //v19;
        break;
    case 0x92u:
        p_Func.push_back("sub_6B4F80((int)&//v294, (int)this, a2, a5, a3);");
        //v126 = p_Func.push_back("sub_6BF840(&//v294);");
        p_Func.push_back("sub_6A1F10(&//v294);");
        //result = //v126;
        break;
    case 0x94u:
        p_Func.push_back("sub_6DC6A0(&//v276, (int)this, a2, (unsigned int)a5, a3);");
        //v252 = p_Func.push_back("sub_6BF2F0(&//v276);");
        p_Func.push_back("sub_6D71B0(&//v276);");
        //result = //v252;
        break;
    case 0x95u:
        p_Func.push_back("sub_6C4760((int)&//v301, (int)this, a2, a5, a3);");
        //v202 = p_Func.push_back("sub_6BB560(&//v301);");
        p_Func.push_back("sub_6A19C0(&//v301);");
        //result = //v202;
        break;
    case 0x96u:
        p_Func.push_back("sub_6B6DD0((int)&//v298, (int)this, a2, a5, a3);");
        //v101 = p_Func.push_back("sub_6B6E90(&//v298);");
        p_Func.push_back("sub_6A0F40(&//v298);");
        //result = //v101;
        break;
    case 0x97u:
        p_Func.push_back("sub_6B2E00(&//v274, (int)this, a2, a5, a3);");
        //v246 = p_Func.push_back("sub_6AD5C0(&//v274);");
        p_Func.push_back("sub_6A1680(&//v274);");
        //result = //v246;
        break;
    case 0x98u:
        p_Func.push_back("sub_6C6EC0((int)&//v299, (int)this, a2, a5, a3);");
        //v184 = p_Func.push_back("sub_6BFB60(&//v299);");
        p_Func.push_back("sub_6A19A0(&//v299);");
        //result = //v184;
        break;
    case 0x9Au:
        p_Func.push_back("sub_6E1710((int)&//v273, (int)this, a2, (int)a5, a3);");
        //v217 = p_Func.push_back("sub_6AC2C0(&//v273);");
        p_Func.push_back("sub_6CAB30(&//v273);");
        //result = //v217;
        break;
    case 0x9Bu:
        p_Func.push_back("sub_6A2AD0(&//v288, (int)this, a2, a5, a3);");
        //v136 = p_Func.push_back("sub_6A8D50(&//v288);");
        p_Func.push_back("sub_6A0D90(&//v288);");
        //result = //v136;
        break;
    case 0x9Du:
        p_Func.push_back("sub_6BC240((int)&//v301, (int)this, a2, a5, a3);");
        //v206 = p_Func.push_back("sub_6BF380(&//v301);");
        p_Func.push_back("sub_6A1E10(&//v301);");
        //result = //v206;
        break;
    case 0xA1u:
        p_Func.push_back("sub_6A1E30(&//v288, (int)this, a2, a5, a3);");
        //v99 = p_Func.push_back("sub_6AAD80(&//v288);");
        p_Func.push_back("sub_6A2D50(&//v288);");
        //result = //v99;
        break;
    case 0xA2u:
        p_Func.push_back("sub_6BC140((int)&//v299, (int)this, a2, a5, a3);");
        //v59 = p_Func.push_back("sub_6B7E40(&//v299);");
        p_Func.push_back("sub_6A1630(&//v299);");
        //result = //v59;
        break;
    case 0xA3u:
        p_Func.push_back("sub_6E1750(&//v295, (int)this, a2, a5, a3);");
        //v242 = p_Func.push_back("sub_6AC8B0(&//v295);");
        p_Func.push_back("sub_6E5A30(&//v295);");
        //result = //v242;
        break;
    case 0xA5u:
        p_Func.push_back("sub_6D0CE0((int)&//v299, (int)this, a2, a5, a3);");
        //v156 = p_Func.push_back("sub_6B31B0(&//v299);");
        p_Func.push_back("sub_6A1DF0(&//v299);");
        //result = //v156;
        break;
    case 0xAAu:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v64 = p_Func.push_back("sub_6B04D0(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v64;
        break;
    case 0xABu:
        p_Func.push_back("sub_6CB630((int)&//v297, (int)this, a2, a5, a3);");
        //v56 = p_Func.push_back("sub_6AA0C0(&//v297);");
        p_Func.push_back("sub_6A29A0(&//v297);");
        //result = //v56;
        break;
    case 0xAFu:
        p_Func.push_back("sub_6CB670((int)&//v299, (int)this, a2, a5, a3);");
        //v171 = p_Func.push_back("sub_6ABE50(&//v299);");
        p_Func.push_back("sub_6A1620(&//v299);");
        //result = //v171;
        break;
    case 0xB0u:
        p_Func.push_back("sub_6C4820(&//v292, (int)this, a2, a5, a3);");
        //v80 = p_Func.push_back("sub_6BEDA0(&//v292);");
        p_Func.push_back("sub_6A2120(&//v292);");
        //result = //v80;
        break;
    case 0xB1u:
        p_Func.push_back("sub_6B0610(&//v289, (int)this, a2, a5, a3);");
        //v48 = p_Func.push_back("sub_6A9080(&//v289);");
        p_Func.push_back("sub_6B81E0(&//v289);");
        //result = //v48;
        break;
    case 0xB3u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v55 = p_Func.push_back("sub_6BBDB0(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v55;
        break;
    case 0xB6u:
        p_Func.push_back("sub_6AD080(&//v300, (int)this, a2, a5, a3);");
        //v37 = p_Func.push_back("sub_6A51B0(&//v300);");
        p_Func.push_back("sub_6A15D0(&//v300);");
        //result = //v37;
        break;
    case 0xB7u:
        p_Func.push_back("sub_6F64C0(&//v299, (unsigned int)this, a2, a5, a3);");
        //v74 = p_Func.push_back("sub_6AE160(&//v299);");
        p_Func.push_back("sub_6F42E0(&//v299);");
        //result = //v74;
        break;
    case 0xB8u:
        p_Func.push_back("sub_6BD0A0(&//v301, (int)this, a2, a5, a3);");
        //v261 = p_Func.push_back("sub_6B44D0(&//v301);");
        p_Func.push_back("sub_6A10B0(&//v301);");
        //result = //v261;
        break;
    case 0xB9u:
        p_Func.push_back("sub_6C6070((int)&//v301, (int)this, a2, a5, a3);");
        //v194 = p_Func.push_back("sub_6C1500(&//v301);");
        p_Func.push_back("sub_6A2270(&//v301);");
        //result = //v194;
        break;
    case 0xBCu:
        p_Func.push_back("sub_6B7DC0((int)&//v299, (int)this, a2, a5, a3);");
        //v63 = p_Func.push_back("sub_6B4510(&//v299);");
        p_Func.push_back("sub_6A1790(&//v299);");
        //result = //v63;
        break;
    case 0xBEu:
        p_Func.push_back("sub_6C4910(&//v302, (int)this, a2, a5, a3);");
        //v260 = p_Func.push_back("sub_6BB7D0(&//v302);");
        p_Func.push_back("sub_6A1F40(&//v302);");
        //result = //v260;
        break;
    case 0xBFu:
        p_Func.push_back("sub_6D7340(&//v271, (int)this, a2, (unsigned int)a5, a3);");
        //v117 = p_Func.push_back("sub_6BED20(&//v271);");
        p_Func.push_back("sub_6D4FB0(&//v271);");
        //result = //v117;
        break;
    case 0xC2u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v264 = p_Func.push_back("sub_6C0500(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v264;
        break;
    case 0xC4u:
        p_Func.push_back("sub_6BAE70(&//v292, (int)this, a2, a5, a3);");
        //v70 = p_Func.push_back("sub_6B1CB0(&//v292);");
        p_Func.push_back("sub_6B0480(&//v292);");
        //result = //v70;
        break;
    case 0xC5u:
        p_Func.push_back("sub_6E3E30(&//v295, (int)this, a2, a5, a3);");
        //v82 = p_Func.push_back("sub_6C1990(&//v295);");
        p_Func.push_back("sub_6E2CC0(&//v295);");
        //result = //v82;
        break;
    case 0xC8u:
        p_Func.push_back("sub_6F0760(&//v296, (int)this, a2, (unsigned int)a5, a3);");
        //v109 = p_Func.push_back("sub_6A41E0(&//v296);");
        p_Func.push_back("sub_6DBA90(&//v296);");
        //result = //v109;
        break;
    case 0xC9u:
        p_Func.push_back("sub_6BE090((int)&//v291, (int)this, a2, (int)a5, a3);");
        //v27 = p_Func.push_back("sub_6A4D80(&//v291);");
        p_Func.push_back("sub_6A0FF0(&//v291);");
        //result = //v27;
        break;
    case 0xCAu:
        p_Func.push_back("sub_6C8320((int)&//v301, (int)this, a2, a5, a3);");
        //v197 = p_Func.push_back("sub_6AEED0(&//v301);");
        p_Func.push_back("sub_6A2D60(&//v301);");
        //result = //v197;
        break;
    case 0xCBu:
        p_Func.push_back("sub_6C4F10((int)&//v299, (int)this, a2, a5, a3);");
        //v155 = p_Func.push_back("sub_6BF4B0(&//v299);");
        p_Func.push_back("sub_6A19E0(&//v299);");
        //result = //v155;
        break;
    case 0xCDu:
        p_Func.push_back("sub_6A2DA0(&//v272, (int)this, a2, a5, a3);");
        //v263 = p_Func.push_back("sub_6A9220(&//v272);");
        p_Func.push_back("sub_6A1990(&//v272);");
        //result = //v263;
        break;
    case 0xCEu:
        p_Func.push_back("sub_6F82D0(&//v292, (int)this, a2, a5, a3);");
        //v249 = p_Func.push_back("sub_6AD440(&//v292);");
        p_Func.push_back("sub_6F64A0(&//v292);");
        //result = //v249;
        break;
    case 0xD0u:
        p_Func.push_back("sub_6B83E0((int)&//v299, (int)this, a2, a5, a3);");
        //v154 = p_Func.push_back("sub_6AB3F0(&//v299);");
        p_Func.push_back("sub_6A2690(&//v299);");
        //result = //v154;
        break;
    case 0xD1u:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v124 = p_Func.push_back("sub_6C1320(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v124;
        break;
    case 0xD3u:
        p_Func.push_back("sub_6A22E0(&//v298, (int)this, a2, a5, a3);");
        //v131 = p_Func.push_back("sub_6A8CB0(&//v298);");
        p_Func.push_back("sub_6A2680(&//v298);");
        //result = //v131;
        break;
    case 0xD4u:
        p_Func.push_back("sub_6C8720((int)&//v299, (int)this, a2, a5, a3);");
        //v152 = p_Func.push_back("sub_6BF8D0(&//v299);");
        p_Func.push_back("sub_6A1130(&//v299);");
        //result = //v152;
        break;
    case 0xD6u:
        p_Func.push_back("sub_6C0A50((int)&//v297, (int)this, a2, a5, a3);");
        //v42 = p_Func.push_back("sub_6AB430(&//v297);");
        p_Func.push_back("sub_6A2EE0(&//v297);");
        //result = //v42;
        break;
    case 0xDBu:
        p_Func.push_back("sub_6C60C0(&//v281, (int)this, a2, (unsigned int)a5, a3);");
        //v83 = p_Func.push_back("sub_6B3E10(&//v281);");
        p_Func.push_back("sub_6A2A90(&//v281);");
        //result = //v83;
        break;
    case 0xDFu:
        p_Func.push_back("sub_6DEB60(&//v299, (int)this, a2, a5, a3);");
        //v127 = p_Func.push_back("sub_6A5CC0(&//v299);");
        p_Func.push_back("sub_6E5A60(&//v299);");
        //result = //v127;
        break;
    case 0xE1u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v15 = p_Func.push_back("sub_6A3C80(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v15;
        break;
    case 0xE4u:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v111 = p_Func.push_back("sub_6A5C80(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v111;
        break;
    case 0xE5u:
        p_Func.push_back("sub_6B35E0(&//v285, (int)this, a2, a5, a3);");
        //v84 = p_Func.push_back("sub_6B6D40(&//v285);");
        p_Func.push_back("sub_6A12C0(&//v285);");
        //result = //v84;
        break;
    case 0xE6u:
        p_Func.push_back("sub_6B8430((int)&//v301, (int)this, a2, a5, a3);");
        //v193 = p_Func.push_back("sub_6AFA20(&//v301);");
        p_Func.push_back("sub_6A28A0(&//v301);");
        //result = //v193;
        break;
    case 0xE8u:
        p_Func.push_back("sub_6C1360((int)&//v301, (int)this, a2, a5, a3);");
        //v195 = p_Func.push_back("sub_6C1730(&//v301);");
        p_Func.push_back("sub_6A2320(&//v301);");
        //result = //v195;
        break;
    case 0xE9u:
        p_Func.push_back("sub_6C4910(&//v302, (int)this, a2, a5, a3);");
        //v233 = p_Func.push_back("sub_6B93B0(&//v302);");
        p_Func.push_back("sub_6A1F40(&//v302);");
        //result = //v233;
        break;
    case 0xEDu:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v266 = p_Func.push_back("sub_6A6D70(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v266;
        break;
    case 0xF1u:
        p_Func.push_back("sub_6C5F60((int)&//v299, (int)this, a2, a5, a3);");
        //v178 = p_Func.push_back("sub_6B2080(&//v299);");
        p_Func.push_back("sub_6A1E00(&//v299);");
        //result = //v178;
        break;
    case 0xF3u:
        p_Func.push_back("sub_6F4190(&//v270, (int)this, a2, (int)a5, a3);");
        //v29 = p_Func.push_back("sub_6A5B70(&//v270);");
        p_Func.push_back("sub_6DAE30(&//v270);");
        //result = //v29;
        break;
    case 0xF4u:
        p_Func.push_back("sub_6B7E00((int)&//v297, (int)this, a2, a5, a3);");
        //v68 = p_Func.push_back("sub_6B4FC0(&//v297);");
        p_Func.push_back("sub_6A0DA0(&//v297);");
        //result = //v68;
        break;
    case 0xF5u:
        p_Func.push_back("sub_6C78D0((int)&//v301, (int)this, a2, a5, a3);");
        //v196 = p_Func.push_back("sub_6AAD00(&//v301);");
        p_Func.push_back("sub_6A2E30(&//v301);");
        //result = //v196;
        break;
    case 0xF6u:
        p_Func.push_back("sub_6D9700(&//v282, (int)this, a2, a5, a3);");
        //v138 = p_Func.push_back("sub_6AA000(&//v282);");
        p_Func.push_back("sub_6A15B0(&//v282);");
        //result = //v138;
        break;
    case 0xF7u:
        p_Func.push_back("sub_6C87A0((int)&//v301, (int)this, a2, a5, a3);");
        //v201 = p_Func.push_back("sub_6BF6F0(&//v301);");
        p_Func.push_back("sub_6A2D80(&//v301);");
        //result = //v201;
        break;
    case 0xF8u:
        p_Func.push_back("sub_6A2A20(&//v293, (int)this, a2, a5, a3);");
        //v16 = p_Func.push_back("sub_6B5720(&//v293);");
        p_Func.push_back("sub_6A0DE0(&//v293);");
        //result = //v16;
        break;
    case 0xF9u:
        p_Func.push_back("sub_6B08A0(&//v297, (int)this, a2, a5, a3);");
        //v248 = p_Func.push_back("sub_6B6520(&//v297);");
        p_Func.push_back("sub_6A0F50(&//v297);");
        //result = //v248;
        break;
    case 0xFAu:
        p_Func.push_back("sub_6D9280(&//v283, (int)this, a2, (int)a5, a3);");
        //v149 = p_Func.push_back("sub_6BECD0(&//v283);");
        p_Func.push_back("sub_6A2DE0(&//v283);");
        //result = //v149;
        break;
    case 0xFBu:
        p_Func.push_back("sub_6D3070((int)&//v299, (int)this, a2, a5, a3);");
        //v151 = p_Func.push_back("sub_6B7B60(&//v299);");
        p_Func.push_back("sub_6A17B0(&//v299);");
        //result = //v151;
        break;
    case 0xFCu:
        p_Func.push_back("sub_6B5C90((int)&//v299, (int)this, a2, a5, a3);");
        //v225 = p_Func.push_back("sub_6BA460(&//v299);");
        p_Func.push_back("sub_6A1040(&//v299);");
        //result = //v225;
        break;
    case 0xFDu:
        p_Func.push_back("sub_6CA860((int)&//v299, (int)this, a2, a5, a3);");
        //v159 = p_Func.push_back("sub_6AE220(&//v299);");
        p_Func.push_back("sub_6A24C0(&//v299);");
        //result = //v159;
        break;
    case 0xFFu:
        p_Func.push_back("sub_6D7160(&//v289, (int)this, a2, a5, a3);");
        //v219 = p_Func.push_back("sub_6A72E0(&//v289);");
        p_Func.push_back("sub_6D0470(&//v289);");
        //result = //v219;
        break;
    case 0x101u:
        p_Func.push_back("sub_6C6F50((int)&//v301, (int)this, a2, a5, a3);");
        //v102 = p_Func.push_back("sub_6B6E50(&//v301);");
        p_Func.push_back("sub_6A15C0(&//v301);");
        //result = //v102;
        break;
    case 0x102u:
        p_Func.push_back("sub_6C4860((int)&//v301, (int)this, a2, a5, a3);");
        //v200 = p_Func.push_back("sub_6A6D20(&//v301);");
        p_Func.push_back("sub_6A26B0(&//v301);");
        //result = //v200;
        break;
    case 0x105u:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v129 = p_Func.push_back("sub_6B41E0(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v129;
        break;
    case 0x106u:
        p_Func.push_back("sub_6CF060((int)&//v299, (int)this, a2, a5, a3);");
        //v97 = p_Func.push_back("sub_6C07C0(&//v299);");
        p_Func.push_back("sub_6A0E00(&//v299);");
        //result = //v97;
        break;
    case 0x108u:
        p_Func.push_back("sub_6B7830((int)&//v301, (int)this, a2, a5, a3);");
        //v53 = p_Func.push_back("sub_6B31F0(&//v301);");
        p_Func.push_back("sub_6A2440(&//v301);");
        //result = //v53;
        break;
    case 0x109u:
        p_Func.push_back("sub_6DD5D0(&//v299, (int)this, a2, (unsigned int)a5, a3);");
        //v215 = p_Func.push_back("sub_6ACB90(&//v299);");
        p_Func.push_back("sub_6DD580(&//v299);");
        //result = //v215;
        break;
    case 0x10Au:
        p_Func.push_back("sub_6D9110(&//v283, (int)this, a2, a5, a3);");
        //v148 = p_Func.push_back("sub_6BC810(&//v283);");
        p_Func.push_back("sub_6A2EF0(&//v283);");
        //result = //v148;
        break;
    case 0x10Cu:
        p_Func.push_back("sub_6C7960((int)&//v297, (int)this, a2, a5, a3);");
        //v231 = p_Func.push_back("sub_6AD340(&//v297);");
        p_Func.push_back("sub_6A2640(&//v297);");
        //result = //v231;
        break;
    case 0x10Du:
        p_Func.push_back("sub_6BE920(&//v292, (int)this, a2, a5, a3);");
        //v79 = p_Func.push_back("sub_6B5440(&//v292);");
        p_Func.push_back("sub_6A2F40(&//v292);");
        //result = //v79;
        break;
    case 0x113u:
        p_Func.push_back("sub_6A1640(&//v299, (int)this, a2, a5, a3);");
        //v38 = p_Func.push_back("sub_6A92A0(&//v299);");
        p_Func.push_back("sub_6A0E10(&//v299);");
        //result = //v38;
        break;
    case 0x116u:
        p_Func.push_back("sub_6DEAD0(&//v295, (int)this, a2, a5, a3);");
        //v228 = p_Func.push_back("sub_6A6DB0(&//v295);");
        p_Func.push_back("sub_6DA920(&//v295);");
        //result = //v228;
        break;
    case 0x117u:
        p_Func.push_back("sub_6A1310(&//v301, (int)this, a2, a5, a3);");
        //v209 = p_Func.push_back("sub_6B0B20(&//v301);");
        p_Func.push_back("sub_6A1290(&//v301);");
        //result = //v209;
        break;
    case 0x119u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v62 = p_Func.push_back("sub_6A5E70(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v62;
        break;
    case 0x11Bu:
        p_Func.push_back("sub_6BCC40((int)&//v299, (int)this, a2, a5, a3);");
        //v175 = p_Func.push_back("sub_6AB850(&//v299);");
        p_Func.push_back("sub_6A0DD0(&//v299);");
        //result = //v175;
        break;
    case 0x11Cu:
        p_Func.push_back("sub_6B9070((int)&//v301, (int)this, a2, a5, a3);");
        //v190 = p_Func.push_back("sub_6BE120(&//v301);");
        p_Func.push_back("sub_6A0FE0(&//v301);");
        //result = //v190;
        break;
    case 0x11Eu:
        p_Func.push_back("sub_6B5D90(&//v299, (int)this, a2, a5, a3);");
        //v173 = p_Func.push_back("sub_6B4560(&//v299);");
        p_Func.push_back("sub_6A1EF0(&//v299);");
        //result = //v173;
        break;
    case 0x11Fu:
        p_Func.push_back("sub_6D4670((int)&//v299, (int)this, a2, a5, a3);");
        //v150 = p_Func.push_back("sub_6B8F00(&//v299);");
        p_Func.push_back("sub_6A0EF0(&//v299);");
        //result = //v150;
        break;
    case 0x121u:
        p_Func.push_back("sub_6F41E0(&//v286, (int)this, a2, a5, a3);");
        //v254 = p_Func.push_back("sub_6BF3C0(&//v286);");
        p_Func.push_back("sub_6E0290(&//v286, (const char *)//v5);");
        //result = //v254;
        break;
    case 0x122u:
        p_Func.push_back("sub_6DACF0(&//v282, (int)this, a2, (int)a5, a3);");
        //v139 = p_Func.push_back("sub_6BCD10(&//v282);");
        p_Func.push_back("sub_6A1F20(&//v282);");
        //result = //v139;
        break;
    case 0x123u:
        p_Func.push_back("sub_6A2B60(&//v295, (int)this, a2, a5, a3);");
        //v76 = p_Func.push_back("sub_6B5CE0(&//v295);");
        p_Func.push_back("sub_6A1C90(&//v295);");
        //result = //v76;
        break;
    case 0x126u:
        p_Func.push_back("sub_6AEF10(&//v301, (int)this, a2, a5, a3);");
        //v120 = p_Func.push_back("sub_6A6650(&//v301);");
        p_Func.push_back("sub_6B1AE0(&//v301);");
        //result = //v120;
        break;
    case 0x12Au:
        p_Func.push_back("sub_6B2750(&//v293, (int)this, a2, a5, a3);");
        //v224 = p_Func.push_back("sub_6BE300(&//v293);");
        p_Func.push_back("sub_6B19D0(&//v293);");
        //result = //v224;
        break;
    case 0x12Du:
        p_Func.push_back("sub_6AD080(&//v300, (int)this, a2, a5, a3);");
        //v265 = p_Func.push_back("sub_6B2040(&//v300);");
        p_Func.push_back("sub_6A15D0(&//v300);");
        //result = //v265;
        break;
    case 0x12Eu:
        p_Func.push_back("sub_6CB4C0((int)&//v301, (int)this, a2, a5, a3);");
        //v205 = p_Func.push_back("sub_6BE880(&//v301);");
        p_Func.push_back("sub_6A1FF0(&//v301);");
        //result = //v205;
        break;
    case 0x12Fu:
        p_Func.push_back("sub_6E5820(&//v299, (unsigned int)this, a2, a5, a3);");
        //v12 = p_Func.push_back("sub_6BBF50(&//v299);");
        p_Func.push_back("sub_6D9310(&//v299);");
        //result = //v12;
        break;
    case 0x130u:
        p_Func.push_back("sub_6C6110((int)&//v292, (int)this, a2, a5, a3);");
        //v177 = p_Func.push_back("sub_6AC120(&//v292);");
        p_Func.push_back("sub_6CCA20(&//v292);");
        //result = //v177;
        break;
    case 0x132u:
        p_Func.push_back("sub_6C0020((int)&//v297, (int)this, a2, a5, a3);");
        //v114 = p_Func.push_back("sub_6BC580(&//v297);");
        p_Func.push_back("sub_6A1F90(&//v297);");
        //result = //v114;
        break;
    case 0x134u:
        p_Func.push_back("sub_6C1180(&//v295, (int)this, a2, a5, a3);");
        //v92 = p_Func.push_back("sub_6B07E0(&//v295);");
        p_Func.push_back("sub_6A1DB0(&//v295);");
        //result = //v92;
        break;
    case 0x135u:
        p_Func.push_back("sub_6C1130(&//v297, (int)this, a2, a5, a3);");
        //v122 = p_Func.push_back("sub_6C0920(&//v297);");
        p_Func.push_back("sub_6A28B0(&//v297);");
        //result = //v122;
        break;
    case 0x137u:
        p_Func.push_back("sub_6BE7A0(&//v300, (int)this, a2, a5, a3);");
        //v71 = p_Func.push_back("sub_6C0DE0(&//v300);");
        p_Func.push_back("sub_6A2280(&//v300);");
        //result = //v71;
        break;
    case 0x139u:
        p_Func.push_back("sub_6C3FD0((int)&//v299, (int)this, a2, a5, a3);");
        //v81 = p_Func.push_back("sub_6BFF80(&//v299);");
        p_Func.push_back("sub_6A1F00(&//v299);");
        //result = //v81;
        break;
    case 0x13Eu:
        p_Func.push_back("sub_6B3230(&//v301, (int)this, a2, a5, a3);");
        //v91 = p_Func.push_back("sub_6AB7A0(&//v301);");
        p_Func.push_back("sub_6A2650(&//v301);");
        //result = //v91;
        break;
    case 0x141u:
        p_Func.push_back("sub_6AA2E0(&//v301, (int)this, a2, a5, a3);");
        //v8 = p_Func.push_back("sub_6B2BD0(&//v301);");
        p_Func.push_back("sub_6A1CD0(&//v301);");
        //result = //v8;
        break;
    case 0x142u:
        p_Func.push_back("sub_6A2200(&//v301, (int)this, a2, a5, a3);");
        //v262 = p_Func.push_back("sub_6ABD00(&//v301);");
        p_Func.push_back("sub_6A1E70(&//v301);");
        //result = //v262;
        break;
    case 0x144u:
        p_Func.push_back("sub_6C4910(&//v302, (int)this, a2, a5, a3);");
        //v23 = p_Func.push_back("sub_6A4A80(&//v302);");
        p_Func.push_back("sub_6A1F40(&//v302);");
        //result = //v23;
        break;
    case 0x146u:
        p_Func.push_back("sub_6B2830((int)&//v301, (int)this, a2, a5, a3);");
        //v207 = p_Func.push_back("sub_6BAF00(&//v301);");
        p_Func.push_back("sub_6A0F60(&//v301);");
        //result = //v207;
        break;
    case 0x149u:
        p_Func.push_back("sub_6ECA00(&//v295, (unsigned int)this, a2, (int)a5, a3);");
        //v10 = p_Func.push_back("sub_6ADAE0(&//v295);");
        p_Func.push_back("sub_6E22E0(&//v295);");
        //result = //v10;
        break;
    case 0x14Cu:
        p_Func.push_back("sub_6ECDA0(&//v295, (unsigned int)this, a2, a5, a3);");
        //v52 = p_Func.push_back("sub_6A71D0(&//v295);");
        p_Func.push_back("sub_6E5400(&//v295);");
        //result = //v52;
        break;
    case 0x150u:
        p_Func.push_back("sub_6C4720(&//v290, (int)this, a2, a5, a3);");
        //v46 = p_Func.push_back("sub_6B12F0(&//v290);");
        p_Func.push_back("sub_6C0D90(&//v290);");
        //result = //v46;
        break;
    case 0x151u:
        p_Func.push_back("sub_6B0AA0((int)&//v299, (int)this, a2, a5, a3);");
        //v153 = p_Func.push_back("sub_6BC7D0(&//v299);");
        p_Func.push_back("sub_6A2400(&//v299);");
        //result = //v153;
        break;
    case 0x152u:
        p_Func.push_back("sub_6C9A50((int)&//v299, (int)this, a2, a5, a3);");
        //v180 = p_Func.push_back("sub_6A4890(&//v299);");
        p_Func.push_back("sub_6A0F10(&//v299);");
        //result = //v180;
        break;
    case 0x153u:
        p_Func.push_back("sub_6DA970(&//v282, (int)this, a2, a5, a3);");
        //v146 = p_Func.push_back("sub_6BED60(&//v282);");
        p_Func.push_back("sub_6A2EC0(&//v282);");
        //result = //v146;
        break;
    case 0x155u:
        p_Func.push_back("sub_6A6B80(&//v300, (int)this, a2, a5, a3);");
        //v113 = p_Func.push_back("sub_6B64E0(&//v300);");
        p_Func.push_back("sub_6A13A0(&//v300);");
        //result = //v113;
        break;
    case 0x156u:
        p_Func.push_back("sub_6CB6B0((int)&//v301, (int)this, a2, a5, a3);");
        //v54 = p_Func.push_back("sub_6AB480(&//v301);");
        p_Func.push_back("sub_6A0F30(&//v301);");
        //result = //v54;
        break;
    case 0x158u:
        p_Func.push_back("sub_6C4580((int)&//v296, (int)this, a2, a5, a3);");
        //v14 = p_Func.push_back("sub_6BFD40(&//v296);");
        p_Func.push_back("sub_6A2A00(&//v296);");
        //result = //v14;
        break;
    case 0x15Au:
        p_Func.push_back("sub_6CF2A0((int)&//v301, (int)this, a2, a5, a3);");
        //v100 = p_Func.push_back("sub_6BAD90(&//v301);");
        p_Func.push_back("sub_6A24D0(&//v301);");
        //result = //v100;
        break;
    case 0x15Cu:
        p_Func.push_back("sub_6D8620(&//v297, (int)this, a2, a5, a3);");
        //v235 = p_Func.push_back("sub_6B6020(&//v297);");
        p_Func.push_back("sub_6C9A90(&//v297);");
        //result = //v235;
        break;
    case 0x15Du:
        p_Func.push_back("sub_6D64D0(&//v282, (int)this, a2, a5, a3);");
        //v140 = p_Func.push_back("sub_6A5AD0(&//v282);");
        p_Func.push_back("sub_6A2260(&//v282);");
        //result = //v140;
        break;
    case 0x160u:
        p_Func.push_back("sub_6BF410((int)&//v299, (int)this, a2, a5, a3);");
        //v57 = p_Func.push_back("sub_6B56E0(&//v299);");
        p_Func.push_back("sub_6A17C0(&//v299);");
        //result = //v57;
        break;
    case 0x163u:
        p_Func.push_back("sub_6B1AF0((int)&//v292, (int)this, a2, a5, a3);");
        //v25 = p_Func.push_back("sub_6C08D0(&//v292);");
        p_Func.push_back("sub_6A0EE0(&//v292);");
        //result = //v25;
        break;
    case 0x166u:
        p_Func.push_back("sub_6C0C40((int)&//v299, (int)this, a2, a5, a3);");
        //v162 = p_Func.push_back("sub_6B5400(&//v299);");
        p_Func.push_back("sub_6A2FC0(&//v299);");
        //result = //v162;
        break;
    case 0x16Au:
        p_Func.push_back("sub_6F74F0((int)&//v279, (int)this, a2, a5, a3);");
        //v98 = p_Func.push_back("sub_6C10E0(&//v279);");
        p_Func.push_back("sub_6F4180(&//v279);");
        //result = //v98;
        break;
    case 0x16Bu:
        p_Func.push_back("sub_6C7F40((int)&//v299, (int)this, a2, a5, a3);");
        //v174 = p_Func.push_back("sub_6B6450(&//v299);");
        p_Func.push_back("sub_6A2450(&//v299);");
        //result = //v174;
        break;
    case 0x16Cu:
        p_Func.push_back("sub_6ADBE0((int)&//v295, (int)this, a2, a5, a3);");
        //v176 = p_Func.push_back("sub_6A7540(&//v295);");
        p_Func.push_back("sub_6A23E0(&//v295);");
        //result = //v176;
        break;
    case 0x170u:
        p_Func.push_back("sub_6B93F0((int)&//v295, (int)this, a2, a5, a3);");
        //v22 = p_Func.push_back("sub_6AB240(&//v295);");
        p_Func.push_back("sub_6A0ED0(&//v295);");
        //result = //v22;
        break;
    case 0x171u:
        p_Func.push_back("sub_6D90D0(&//v282, (int)this, a2, a5, a3);");
        //v144 = p_Func.push_back("sub_6C09B0(&//v282);");
        p_Func.push_back("sub_6A1970(&//v282);");
        //result = //v144;
        break;
    case 0x173u:
        p_Func.push_back("sub_6B3560((int)&//v301, (int)this, a2, a5, a3);");
        //v107 = p_Func.push_back("sub_6A97D0(&//v301);");
        p_Func.push_back("sub_6A14E0(&//v301);");
        //result = //v107;
        break;
    case 0x174u:
        p_Func.push_back("sub_6A24E0(&//v297, (int)this, a2, a5, a3);");
        //v221 = p_Func.push_back("sub_6C10A0(&//v297);");
        p_Func.push_back("sub_6A0D60(&//v297);");
        //result = //v221;
        break;
    case 0x177u:
        p_Func.push_back("sub_6A26C0(&//v302, (int)this, a2, a5, a3);");
        //v77 = p_Func.push_back("sub_6AC820(&//v302);");
        p_Func.push_back("sub_6A2660(&//v302);");
        //result = //v77;
        break;
    case 0x179u:
        p_Func.push_back("sub_6D9620((int)&//v284, (int)this, a2, a5, a3);");
        //v93 = p_Func.push_back("sub_6BB780(&//v284);");
        p_Func.push_back("sub_6DA120(&//v284);");
        //result = //v93;
        break;
    case 0x17Cu:
        p_Func.push_back("sub_6B3230(&//v301, (int)this, a2, a5, a3);");
        //v241 = p_Func.push_back("sub_6B6060(&//v301);");
        p_Func.push_back("sub_6A2650(&//v301);");
        //result = //v241;
        break;
    case 0x17Du:
        p_Func.push_back("sub_6B5000((int)&//v301, (int)this, a2, a5, a3);");
        //v192 = p_Func.push_back("sub_6BF130(&//v301);");
        p_Func.push_back("sub_6A2110(&//v301);");
        //result = //v192;
        break;
    case 0x17Eu:
        p_Func.push_back("sub_6C98F0((int)&//v294, (int)this, a2, a5, a3);");
        //v6 = p_Func.push_back("sub_6B72D0(&//v294);");
        p_Func.push_back("sub_6CCA80(&//v294);");
        //result = //v6;
        break;
    case 0x180u:
        p_Func.push_back("sub_6BE960(&//v280, (int)this, a2, a5, a3);");
        //v20 = p_Func.push_back("sub_6ABE90(&//v280);");
        p_Func.push_back("sub_6A5600(&//v280);");
        //result = //v20;
        break;
    case 0x181u:
        p_Func.push_back("sub_6B7D00(&//v299, (int)this, a2, a5, a3);");
        //v187 = p_Func.push_back("sub_6ABE10(&//v299);");
        p_Func.push_back("sub_6A1110(&//v299);");
        //result = //v187;
        break;
    case 0x182u:
        p_Func.push_back("sub_6F2500(&//v290, (int)this, a2, a5, a3);");
        //v24 = p_Func.push_back("sub_6B1600(&//v290);");
        p_Func.push_back("sub_6EE960(&//v290);");
        //result = //v24;
        break;
    case 0x184u:
        p_Func.push_back("sub_6B6E10((int)&//v301, (int)this, a2, a5, a3);");
        //v212 = p_Func.push_back("sub_6B6D80(&//v301);");
        p_Func.push_back("sub_6A2E20(&//v301);");
        //result = //v212;
        break;
    case 0x187u:
        p_Func.push_back("sub_6BD0A0(&//v301, (int)this, a2, a5, a3);");
        //v104 = p_Func.push_back("sub_6A6E40(&//v301);");
        p_Func.push_back("sub_6A10B0(&//v301);");
        //result = //v104;
        break;
    case 0x188u:
        p_Func.push_back("sub_6C45E0((int)&//v301, (int)this, a2, a5, a3);");
        //v44 = p_Func.push_back("sub_6AE720(&//v301);");
        p_Func.push_back("sub_6A0DC0(&//v301);");
        //result = //v44;
        break;
    case 0x189u:
        p_Func.push_back("sub_6D97B0(&//v299, (int)this, a2, a5, a3);");
        //v125 = p_Func.push_back("sub_6BF4F0(&//v299);");
        p_Func.push_back("sub_6C50F0(&//v299);");
        //result = //v125;
        break;
    case 0x18Fu:
        p_Func.push_back("sub_6C0A90((int)&//v301, (int)this, a2, a5, a3);");
        //v67 = p_Func.push_back("sub_6A8160(&//v301);");
        p_Func.push_back("sub_6A1220(&//v301);");
        //result = //v67;
        break;
    case 0x195u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v133 = p_Func.push_back("sub_6A9B20(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v133;
        break;
    case 0x196u:
        p_Func.push_back("sub_6C8410((int)&//v299, (int)this, a2, a5, a3);");
        //v163 = p_Func.push_back("sub_6B8160(&//v299);");
        p_Func.push_back("sub_6A2B10(&//v299);");
        //result = //v163;
        break;
    case 0x197u:
        p_Func.push_back("sub_6BF740(&//v292, (int)this, a2, a5, a3);");
        //v50 = p_Func.push_back("sub_6AF1B0(&//v292);");
        p_Func.push_back("sub_6A29F0(&//v292);");
        //result = //v50;
        break;
    case 0x199u:
        p_Func.push_back("sub_6C3DC0((int)&//v299, (int)this, a2, a5, a3);");
        //v160 = p_Func.push_back("sub_6ACE70(&//v299);");
        p_Func.push_back("sub_6A2330(&//v299);");
        //result = //v160;
        break;
    case 0x19Du:
        p_Func.push_back("sub_6D95B0(&//v288, (int)this, a2, a5, a3);");
        //v51 = p_Func.push_back("sub_6A5EB0(&//v288);");
        p_Func.push_back("sub_6DAD30(&//v288);");
        //result = //v51;
        break;
    case 0x19Eu:
        p_Func.push_back("sub_6E9850(&//v299, (int)this, a2, a5, a3);");
        //v41 = p_Func.push_back("sub_6AAF60(&//v299);");
        p_Func.push_back("sub_6E5330(&//v299);");
        //result = //v41;
        break;
    case 0x1A0u:
        p_Func.push_back("sub_6D4FD0(&//v282, (int)this, a2, a5, a3);");
        //v143 = p_Func.push_back("sub_6A76E0(&//v282);");
        p_Func.push_back("sub_6A19D0(&//v282);");
        //result = //v143;
        break;
    case 0x1A3u:
        p_Func.push_back("sub_6A2810(&//v300, (int)this, a2, a5, a3);");
        //v40 = p_Func.push_back("sub_6B4760(&//v300);");
        p_Func.push_back("sub_6A2670(&//v300);");
        //result = //v40;
        break;
    case 0x1A4u:
        p_Func.push_back("sub_6BF890(&//v288, (int)this, a2, a5, a3);");
        //v47 = p_Func.push_back("sub_6A9820(&//v288);");
        p_Func.push_back("sub_6ADC20(&//v288);");
        //result = //v47;
        break;
    case 0x1A6u:
        p_Func.push_back("sub_6D1EB0(&//v284, (int)this, a2, a5, a3);");
        //v220 = p_Func.push_back("sub_6AC860(&//v284);");
        p_Func.push_back("sub_6A0EC0(&//v284);");
        //result = //v220;
        break;
    case 0x1A7u:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v75 = p_Func.push_back("sub_6B47E0(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v75;
        break;
    case 0x1A8u:
        p_Func.push_back("sub_6BE830((int)&//v301, (int)this, a2, a5, a3);");
        //v106 = p_Func.push_back("sub_6BC180(&//v301);");
        p_Func.push_back("sub_6A0DB0(&//v301);");
        //result = //v106;
        break;
    case 0x1A9u:
        p_Func.push_back("sub_6C98B0((int)&//v299, (int)this, a2, a5, a3);");
        //v216 = p_Func.push_back("sub_6B9030(&//v299);");
        p_Func.push_back("sub_6A2370(&//v299);");
        //result = //v216;
        break;
    case 0x1ADu:
        p_Func.push_back("sub_6F2300(&//v295, (int)this, a2, a5, a3);");
        //v255 = p_Func.push_back("sub_6A32B0(&//v295);");
        p_Func.push_back("sub_6E97C0(&//v295);");
        //result = //v255;
        break;
    case 0x1AEu:
        p_Func.push_back("sub_6DEB20(&//v299, (int)this, a2, a5, a3);");
        //v247 = p_Func.push_back("sub_6A3C10(&//v299);");
        p_Func.push_back("sub_6D9240(&//v299);");
        //result = //v247;
        break;
    case 0x1B0u:
        p_Func.push_back("sub_6F07E0(&//v297, (int)this, a2, a5, a3);");
        //v188 = p_Func.push_back("sub_6AA140(&//v297);");
        p_Func.push_back("sub_6E1F90(&//v297);");
        //result = //v188;
        break;
    case 0x1B1u:
        p_Func.push_back("sub_6F8280(&//v299, (unsigned int)this, a2, a5, a3);");
        //v65 = p_Func.push_back("sub_6B5090(&//v299);");
        p_Func.push_back("sub_6EE990(&//v299);");
        //result = //v65;
        break;
    case 0x1B2u:
        p_Func.push_back("sub_6C9A10(&//v289, (int)this, a2, a5, a3);");
        //v73 = p_Func.push_back("sub_6A75F0(&//v289);");
        p_Func.push_back("sub_6B5D80(&//v289);");
        //result = //v73;
        break;
    case 0x1B3u:
        p_Func.push_back("sub_6D7730(&//v267, (int)this, a2, (int)a5, a3);");
        //v244 = p_Func.push_back("sub_6B4220(&//v267);");
        p_Func.push_back("sub_6A0D80(&//v267);");
        //result = //v244;
        break;
    case 0x1B7u:
        p_Func.push_back("sub_6BC860((int)&//v301, (int)this, a2, a5, a3);");
        //v204 = p_Func.push_back("sub_6B5230(&//v301);");
        p_Func.push_back("sub_6A17A0(&//v301);");
        //result = //v204;
        break;
    case 0x1B8u:
        p_Func.push_back("sub_6B47A0((int)&//v299, (int)this, a2, a5, a3);");
        //v167 = p_Func.push_back("sub_6AB890(&//v299);");
        p_Func.push_back("sub_6A0EB0(&//v299);");
        //result = //v167;
        break;
    case 0x1B9u:
        p_Func.push_back("sub_6BBFA0((int)&//v299, (int)this, a2, a5, a3);");
        //v170 = p_Func.push_back("sub_6AA080(&//v299);");
        p_Func.push_back("sub_6A14F0(&//v299);");
        //result = //v170;
        break;
    case 0x1BAu:
        p_Func.push_back("sub_6C6F10((int)&//v299, (int)this, a2, a5, a3);");
        //v182 = p_Func.push_back("sub_6A34B0(&//v299);");
        p_Func.push_back("sub_6A2FB0(&//v299);");
        //result = //v182;
        break;
    case 0x1BCu:
        p_Func.push_back("sub_6E5370(&//v299, (int)this, a2, a5, a3);");
        //v227 = p_Func.push_back("sub_6B06A0(&//v299);");
        p_Func.push_back("sub_6E5980(&//v299);");
        //result = //v227;
        break;
    case 0x1BEu:
        p_Func.push_back("sub_6BDF60((int)&//v301, (int)this, a2, a5, a3);");
        //v191 = p_Func.push_back("sub_6BBD60(&//v301);");
        p_Func.push_back("sub_6A22C0(&//v301);");
        //result = //v191;
        break;
    case 0x1BFu:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v9 = p_Func.push_back("sub_6BDFA0(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v9;
        break;
    case 0x1C2u:
        p_Func.push_back("sub_6C9940(&//v297, (int)this, a2, a5, a3);");
        //v230 = p_Func.push_back("sub_6A5170(&//v297);");
        p_Func.push_back("sub_6A2E70(&//v297);");
        //result = //v230;
        break;
    case 0x1C5u:
        p_Func.push_back("sub_6F73B0(&//v290, (int)this, a2, (int)a5, a3);");
        //v239 = p_Func.push_back("sub_6B1640(&//v290);");
        p_Func.push_back("sub_6F6EE0(&//v290);");
        //result = //v239;
        break;
    case 0x1CCu:
        p_Func.push_back("sub_6B5BB0(&//v289, (int)this, a2, a5, a3);");
        //v250 = p_Func.push_back("sub_6BF080(&//v289);");
        p_Func.push_back("sub_6A7A40(&//v289);");
        //result = //v250;
        break;
    case 0x1CDu:
        p_Func.push_back("sub_6C8660((int)&//v299, (int)this, a2, a5, a3);");
        //v157 = p_Func.push_back("sub_6B35A0(&//v299);");
        p_Func.push_back("sub_6A27B0(&//v299);");
        //result = //v157;
        break;
    case 0x1D2u:
        p_Func.push_back("sub_6D4540(&//v283, (int)this, a2, a5, a3);");
        //v147 = p_Func.push_back("sub_6ABA40(&//v283);");
        p_Func.push_back("sub_6A1F30(&//v283);");
        //result = //v147;
        break;
    case 0x1D5u:
        p_Func.push_back("sub_6C0C80((int)&//v297, (int)this, a2, a5, a3);");
        //v96 = p_Func.push_back("sub_6B48E0(&//v297);");
        p_Func.push_back("sub_6A2FD0(&//v297);");
        //result = //v96;
        break;
    case 0x1D8u:
        p_Func.push_back("sub_6C6F90((int)&//v299, (int)this, a2, a5, a3);");
        //v172 = p_Func.push_back("sub_6AE7D0(&//v299);");
        p_Func.push_back("sub_6A1FE0(&//v299);");
        //result = //v172;
        break;
    case 0x1D9u:
        p_Func.push_back("sub_6A5BF0(&//v298, (int)this, a2, a5, a3);");
        //v87 = p_Func.push_back("sub_6A9260(&//v298);");
        p_Func.push_back("sub_6A0E30(&//v298);");
        //result = //v87;
        break;
    case 0x1DDu:
        p_Func.push_back("sub_6B3230(&//v301, (int)this, a2, a5, a3);");
        //v17 = p_Func.push_back("sub_6BE040(&//v301);");
        p_Func.push_back("sub_6A2650(&//v301);");
        //result = //v17;
        break;
    case 0x1DEu:
        p_Func.push_back("sub_6BC280((int)&//v295, (int)this, a2, a5, a3);");
        //v58 = p_Func.push_back("sub_6A4390(&//v295);");
        p_Func.push_back("sub_6A2130(&//v295);");
        //result = //v58;
        break;
    case 0x1DFu:
        p_Func.push_back("sub_6A7590(&//v301, (int)this, a2, a5, a3);");
        //v112 = p_Func.push_back("sub_6BAE20(&//v301);");
        p_Func.push_back("sub_6A1090(&//v301);");
        //result = //v112;
        break;
    case 0x1E0u:
        p_Func.push_back("sub_6CF0A0(&//v282, (int)this, a2, (int)a5, a3);");
        //v89 = p_Func.push_back("sub_6A5610(&//v282);");
        p_Func.push_back("sub_6A23F0(&//v282);");
        //result = //v89;
        break;
    case 0x1E1u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v251 = p_Func.push_back("sub_6BBD20(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v251;
        break;
    case 0x1E7u:
        p_Func.push_back("sub_6A2E40(&//v302, (int)this, a2, (int)a5, a3);");
        //v258 = p_Func.push_back("sub_6B5D30(&//v302);");
        p_Func.push_back("sub_6A1120(&//v302);");
        //result = //v258;
        break;
    case 0x1E8u:
        p_Func.push_back("sub_6DD810(&//v295, (int)this, a2, a5, a3);");
        //v208 = p_Func.push_back("sub_6AA040(&//v295);");
        p_Func.push_back("sub_6D9770(&//v295);");
        //result = //v208;
        break;
    case 0x1ECu:
        p_Func.push_back("sub_6A1EB0(&//v302, (int)this, a2, a5, a3);");
        //v123 = p_Func.push_back("sub_6B0510(&//v302);");
        p_Func.push_back("sub_6A25E0(&//v302);");
        //result = //v123;
        break;
    case 0x1EFu:
        p_Func.push_back("sub_6C99C0((int)&//v299, (int)this, a2, a5, a3);");
        //v21 = p_Func.push_back("sub_6B0820(&//v299);");
        p_Func.push_back("sub_6A18E0(&//v299);");
        //result = //v21;
        break;
    case 0x1F0u:
        p_Func.push_back("sub_6C05B0((int)&//v299, (int)this, a2, a5, a3);");
        //v168 = p_Func.push_back("sub_6B3C10(&//v299);");
        p_Func.push_back("sub_6A1B70(&//v299);");
        //result = //v168;
        break;
    case 0x1F1u:
        p_Func.push_back("sub_6AAD40(&//v297, (int)this, a2, a5, a3);");
        //v49 = p_Func.push_back("sub_6A7A00(&//v297);");
        p_Func.push_back("sub_6A29E0(&//v297);");
        //result = //v49;
        break;
    case 0x1F5u:
        p_Func.push_back("sub_6C5FF0((int)&//v301, (int)this, a2, a5, a3);");
        //v199 = p_Func.push_back("sub_6ABC30(&//v301);");
        p_Func.push_back("sub_6A26A0(&//v301);");
        //result = //v199;
        break;
    case 0x1F6u:
        p_Func.push_back("sub_6EA9A0(&//v288, (int)this, a2, a5, a3);");
        //v253 = p_Func.push_back("sub_6B77F0(&//v288);");
        p_Func.push_back("sub_6E1DE0(&//v288);");
        //result = //v253;
        break;
    case 0x1FAu:
        p_Func.push_back("sub_6CAB80((int)&//v299, (int)this, a2, a5, a3);");
        //v181 = p_Func.push_back("sub_6A5C40(&//v299);");
        p_Func.push_back("sub_6A21B0(&//v299);");
        //result = //v181;
        break;
    case 0x1FBu:
        p_Func.push_back("sub_6C8760(&//v284, (int)this, a2, a5, a3);");
        //v28 = p_Func.push_back("sub_6BD100(&//v284);");
        p_Func.push_back("sub_6A1DC0(&//v284);");
        //result = //v28;
        break;
    case 0x1FCu:
        p_Func.push_back("sub_6C5FB0((int)&//v299, (int)this, a2, a5, a3);");
        //v169 = p_Func.push_back("sub_6C1540(&//v299);");
        p_Func.push_back("sub_6A2800(&//v299);");
        //result = //v169;
        break;
    case 0x1FFu:
        p_Func.push_back("sub_6BC610((int)&//v297, (int)this, a2, a5, a3);");
        //v60 = p_Func.push_back("sub_6B9370(&//v297);");
        p_Func.push_back("sub_6A13B0(&//v297);");
        //result = //v60;
        break;
    default:
        //result = 0;
        break;
    }
}
