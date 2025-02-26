# 14 Apr 2015
Introduction.

This directory contains files for running server on VME CPU to control high voltage crate type of
CAEN SY527. Caenet VME board type of V288 is used as adapter between Caen crate and VME crate.
One V288 board can be used to control more than one Caen crate that connected
with daisy chain over caenet network (we have 2 crates in configuration).
Server communicates with Java GUI over network with TCP/IP protocol.
Two servers are used with different ports to contorl two Caen crates over one caenet with V288 board.

l 





Directories and files.

vxworks.setup  -  setup enviroments of crosscompiler for VME PPC CPU.
	          Before compile source files, first type in bash:
		  source vxworks.setup

sbscdet.boot  - command boot file for VME CPU 'sbscdet'

sy527  - directory with source files of library functions to control SY527 crate.
server - directory with source files of server that runs on VME VPU and communicates with Java GUI. 

sy527:
sy527.c    - file with functions to control Caen crate using V288 caenet board
vmcaenet.h - definitions for sy527.c
mk_sy527   - command line to compile sy527.c for VME PPC CPU (MVME2306)

server:
server.c  - source file for server
mkserver -  command line to compile server.c for VME PPC CPU (MVME2306)

Start programs.

Load library and monitor sy527:
ld < ../sy527/sy527.o

Set V288 board address on VME bus:
v288addr=0xe0600000;

Start monitor for Caen crates:
taskSpawn "caenmon",150,spTaskOptions,10000, monitor, 2, 12,9,0,0

Parameters: 2  - number of connected Caen crates to V288 board (1-4)
	    9  - Caen crate one number (1-99)
	    12 - Caen crate two number (1-99) 


Load server:
ld < ../server/server.o

Start servers:
taskSpawn "srv1",120,spTaskOptions,10000, start, 2012, 12
taskSpawn "srv2",120,spTaskOptions,10000, start, 2009, 9

Parameters: 2012 (2009) - port number
	    12  (9)     - crate number

Description.


Monitor scans in loops  crates, boards, channels to readout values and parameters
from all hv channels. Values from all hv channels are stored in structured array.
Monitor compares readout value from hv channel with corresponded value stored in array.
If there some difference between values it increments summary numbers.

Server process commands from  Java GUI, translates it for hv crate commands and
sends back responses from crate.
Java GUI monitors changes of channel parameters(properties) by polling summary
numbers from crate using commands: "GS", "LS", "PS".
There are 3 types of summary numbers:
  global:   GS - five four digits words(hex),
  logical:  LS - two four digit word(hex) for every module(slot),
  property: PS - one four digit word(hex) for every hv channel property(parameter).
If chanel property is changed, PS number for that property is incremented,
then LS number for coressponded module(board) is incremented and then GS number is incremented.
GUI gets this sequences and found what property in what module(board) have to update, then send
command with corresponded property.
As example "RC S2 MV" - recall measured voltage (MV) for all channels of module in slot #2.
