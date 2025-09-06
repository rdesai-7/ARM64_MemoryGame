# ARM64 Emulator, Assembler, and Memory Game
In this project we have developed an emulator and assembler for a subset of the ARMv8 AArch64 instruction set, written entirely in C. As an extension, we created a memory training game on a Raspberry Pi. Our game was focussed on supporting and strengthening the cognitive health of individuals, in particular those suffering from dementia. Developed in a group of 4, this university project culminated in a presentation to our professor and earned a final grade of 100%.

## Overview 📖

### Emulator and Assembler
The emulator takes a binary input file, then simulates its execution by running a Fetch-Decode-Execute pipeline while managing the state of memory and registers. The assembler translates ARM64 assembly code into machine-readable binary using a two-pass technique.

⚠️ **Please note:** The source code for the emulator and assembler cannot be displayed publicly as it is part of university coursework. It is available upon request. This repository therefore provides **full documentation**, and the source code for our **memory game only**. Sorry for the inconvenience.

### Memory Game
In an effort to support cognitive health and dementia prevention, we wanted to stimulate short-term memory as well as cognitive sequencing, both of which are known to deteriorate in the early stages of dementia. This led us to develop this memory game, which generates random LED sequences for players to remember, then allows players to repeat the sequence using the corresponding buttons. Longer and longer sequences are displayed until failure. Our final product also included a timeout function, forcing users to respond quickly.

During our iterative development process, we worked towards building a game that was interactive, intuitive, and accessible for all. Take a look at our documentation for more details on the project.

## Tech Stack
Our emulator and assembler were built from stratch in C. Our memory game involved using a Raspberry Pi 3, as well as LEDs, buttons, and a breadboard. We also used the GPIOD library to enable our code to run on a Raspberry Pi.

## Full Documentation
Read documentation for our emulator [here](doc/interim_report.pdf) 

Read documentation for our assembler and memory game [here](doc/final_report.pdf)

## Usage
To run the memory game, you will need some hardware. Please note we have implemented a scalable design, making it easy for anyone to change the desired number of LEDs and buttons. By default, it is set to 6, but on line 5 [here](src/memorygame_/game_state.h) you can change that. Scaling the hardware up involves adding more buttons, LEDs, resistors, wires, and possibly breadboards.

**Prerequisites**:
- 1 Raspberry Pi
- 2 Breadboards
- 6 buttons
- 6 LEDs
- 6 resistors (&ge; 220&0mega;)
- ~25 wires (including M2M and M2F)

To set up the hardware, please refer to [this image](doc/hardware_pic.jpg), and [this too](doc/hardware_pic_2.jpg).

**Instructions**
1. Clone the repository to your Raspberry Pi
2. Download the GPIOD library to your Raspberry Pi
3. cd into src/memorygame_
4. Run 'make clean'
5. Run 'make'
6. Now run './memory_game' and press the two outer buttons together to start playing!

Scores are displayed in the terminal, and we encourage adjusting the constants in game_state.h to adjust the difficulty!

Enjoy improving your cognitive health, and don't hesistate to email me at rohandesai706@gmail.com for any questions!

