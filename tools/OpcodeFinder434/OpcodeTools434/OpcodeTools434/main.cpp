#include "JamClient.h"

#include <iostream>

using namespace std;

int main()
{
    while (true)
    {
        cout << "\nEnter your opcode\n";

        string input;
        cin >> input;

        if (input == "EXIT")
            break;

        if (int opcode = atol(input.c_str()))
        {
            vector<string> output;

            JamClient::CalculateOpcode(opcode, output);

            if (output.empty())
            {
                cout << "\nNo Function found";
                continue;
            }

            for (string s : output)
                cout << "\n" << s;
        }
        else
            cout << "\nInvalid input";
    }
    return 1;
}