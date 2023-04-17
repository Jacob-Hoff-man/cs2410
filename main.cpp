#include <string>
#include <iostream>
#include <fstream>
#include <vector>
//
#include "simulator.h"
#include "fetch.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "\n\nhello world\n";
    if (argc == 7) {
        string inpFileName = argv[1];
        int nf = stoi(argv[2]);
        int ni = stoi(argv[3]);
        int nw = stoi(argv[4]);
        int nb = stoi(argv[5]);
        int nr = stoi(argv[6]);
        Simulator * simulator = new Simulator(inpFileName, nf, ni, nw, nb, nr, false);

    } else if (argc == 8) {
        // debug mode enabled
        string inpFileName = argv[1];
        int nf = stoi(argv[2]);
        int ni = stoi(argv[3]);
        int nw = stoi(argv[4]);
        int nb = stoi(argv[5]);
        int nr = stoi(argv[6]);
        bool debugMode = (bool)stoi(argv[7]);

        cout << "INPUT FILE NAME=" << inpFileName <<
        "\n NF=" << nf <<
        "\n NI=" << ni <<
        "\n NW=" << nw <<
        "\n NB=" << nb <<
        "\n NR=" << nr <<
        "\n DEBUGMODE=" << debugMode <<
        "\n";

        cout << "INIT SIMULATOR AND RUN readInputFile(" << inpFileName.c_str() << ")\n";
        Simulator * simulator = new Simulator(inpFileName, nf, ni, nw, nb, nr, true);
        cout << "\nPRINT INIT MEMORIES\n";
        simulator->printMemories();
        cout << "\nPRINT INIT INSTRUCTIONS\n";
        simulator->printSimulatorInstructions();
        cout << "\nsimulator memories length = " << simulator->getMemoriesSize() << "\n";
        cout << "\nsimulator instructions length = " << simulator->getInstructionsSize() << "\n";
        cout << "\nPRINT INIT BRANCHLABELSTABLE\n";
        simulator->printBranchLabelsTable();
        simulator->tickCycleCount();
        simulator->printCurrentCycleCount();
    } else {
        cout << "invalid input parameters.\n    usage= ./main $(INP_FILE_NAME) $(NF) $(NI) $(NW) $(NR) $(NB)\n";
    }
    return 1;
}