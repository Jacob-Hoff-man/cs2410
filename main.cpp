#include "simulator.h"

int main(int argc, char *argv[])
{
    if (argc == 7) {
        const string inpFileName = argv[1];
        const int nf = stoi(argv[2]);
        const int ni = stoi(argv[3]);
        const int nw = stoi(argv[4]);
        const int nb = stoi(argv[5]);
        const int nr = stoi(argv[6]);
        Simulator * simulator = new Simulator(inpFileName, nf, ni, nw, nb, nr, false);
        simulator->execute();

    } else if (argc == 8) {
        // debug mode enabled
        const string inpFileName = argv[1];
        const int nf = stoi(argv[2]);
        const int ni = stoi(argv[3]);
        const int nw = stoi(argv[4]);
        const int nb = stoi(argv[5]);
        const int nr = stoi(argv[6]);
        const bool debugMode = (bool)stoi(argv[7]);

        cout << "INPUT FILE NAME=" << inpFileName <<
        "\n NF=" << nf <<
        "\n NI=" << ni <<
        "\n NW=" << nw <<
        "\n NB=" << nb <<
        "\n NR=" << nr <<
        "\n DEBUGMODE=" << debugMode <<
        "\n";

        if (debugMode) cout << "INIT SIMULATOR AND RUN readInputFile(" << inpFileName.c_str() << ")\n";
        Simulator * simulator = new Simulator(inpFileName, nf, ni, nw, nb, nr, true);
        simulator->execute();
    } else {
        cout << "invalid input parameters.\n    usage= ./main $(INP_FILE_NAME) $(NF) $(NI) $(NW) $(NR) $(NB)\n";
    }
    return 1;
}