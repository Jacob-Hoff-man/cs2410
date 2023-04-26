# CS2410 Computer Architecture


### Out-Of-Order Execution CPU Simulator Using Tomasulo Algorithm and Speculation

#### How To Run:

**compiling and running the program in “run” mode:**

1. Download the project and unzip the files, if necessary.
2. Traverse into the project directory from the terminal/command window.
3. Run the following commands:

```
  make all
  make run
```

**compiling and running the program in “debug” mode:**

1. Download the project and unzip the files, if necessary.
2. Traverse into the project directory from the terminal/command window.
3. Run the following commands:

```
  make all
  make debug
```

**re-compiling the program:**

1. Traverse into the project directory from the terminal/command window.
2. Run the following commands:

```
  make clear
  make all
  make run // or debug
```

**modify command-line input parameters:**

`Makefile` line 3:

```
  ...
  # input parameters - taken from project.pdf
  INP_FILE_NAME="prog.dat"
  NF=4 # number of instructions fetched per cycle by the fetch unit
  NI=16 # instruction queue max capacity of instructions held in the decode unit
  NW=4 # number of instructions issued per cycle to reservation stations
  NB=4 # number of common data buses
  NR=8 # number of entries available in the circular reorder buffer (ROB)
  ...
```

**fine-tune the debug console output for the program:**

Simulator::cyclePipeline (`simulator.cpp` line 259):

```
  ...
   if (debugMode) {
       cout << "\nPOST CYCLE RESULTS\n";
       // debug per cycle
       // printSimulatorMemories();
       printSimulatorDecodeInstructionQueue();
       // printSimulatorBranchLabelsTable();
       // printSimulatorBtbMap();
       // printSimulatorPhysicalRegs();
       // printSimulatorMappingTable();
       // printSimulatorFreeList();
       // printSimulatorMappingTableHistory();
       // printSimulatorFreeListHistory();
       printSimulatorReservationStations();
       printSimulatorROB();
       // printSimulatorStallCounts();
   }
  ...
```

Simulator::execute (`simulator.cpp` line 308):

```
  ...
       // post execution
       printSimulatorMemories();
       // printSimulatorDecodeInstructionQueue();
       // printSimulatorBranchLabelsTable();
       // printSimulatorBtbMap();
       printSimulatorPhysicalRegs();
       printSimulatorMappingTable();
       // printSimulatorFreeList();
       // printSimulatorMappingTableHistory();
       // printSimulatorFreeListHistory();
       // printSimulatorReservationStations();
       // printSimulatorROB();
       printSimulatorStallCounts();
  ...
```

**The [project report](https://github.com/Jacob-Hoff-man/cs2410/blob/main/Project%20Final%20Report%20Jacob%20Hoffman.pdf) is available for more details.**
