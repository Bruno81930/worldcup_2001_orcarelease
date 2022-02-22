#include <iostream.h>
#include "coach_lang.h"

#include "clangparser.h"

int main (int argc, char** argv)
{
    CM_Message* g_pMess = NULL;

    char current_coach_message[255];

    while (cin)
    {
        cin.getline(current_coach_message, 255);

        g_pMess = parse_coach_message(current_coach_message);

        if (g_pMess == NULL)
        {
            cout << " *No message read" << endl;
        }
        else
        {
            cout << " *";
            g_pMess->Print(cout);
            cout << endl;
        }
    }
}
