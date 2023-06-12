# CMPE661-Xilinx-Template
Template for using Vivado and Xilinx SDK with in a version control friendly way. For use with Digilent Zybo Z7-20 ARM/FPGA SoC development board, as used in RIT's CMPE-661 : Hardware and Software Design for Cryptographic Applications. Ideally this allows some future students to spend less time fussing with Xilinx tooling and more time on the Hands-On (lab) exercises.

See [vivado_setup](https://github.com/jhallen/vivado_setup) for structure explanation and usage.

The `testingGCC` directory exists as a place to develop the C code for the projects and avoid working in Xilinx SDK with the actual hardware until absolutely necessary. `startSDK.sh` was written to simplify launching SDK from the command line using the hdf file for the Zybo from HWSW.
