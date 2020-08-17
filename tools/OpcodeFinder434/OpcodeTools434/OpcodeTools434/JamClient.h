#ifndef JAMCLIENT_H
#define JAMCLIENT_H

#include <string>
#include <vector>

class JamClient
{
public:
    static void CalculateOpcode(int p_Opcode, std::vector<std::string>& p_Func);
};

#endif