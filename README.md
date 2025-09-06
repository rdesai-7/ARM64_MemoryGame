# ARM64 Emulator, Assembler, and Memory Game
This project features a custom emulator and assembler for a subset of the ARMv8 AArch64 instruction set, written entirely in C. As a hardware extension, we created an interactive memory training game on a Raspberry Pi. Our game was designed to support and strengthen the cognitive health of individuals, particularly those affected by dementia. Developed in a group of 4, this university project culminated in a presentation to our professor and earned a final grade of 100%.

## Overview 📖

### Emulator and Assembler
The emulator takes a binary input file, then simulates its execution by running a Fetch-Decode-Execute pipeline while managing the state of memory and registers. The assembler translates ARM64 assembly code into machine-readable binary using a two-pass technique.

⚠️ **Please note:** The source code for the emulator and assembler cannot be displayed publicly as it is part of university coursework. It is available upon request. This repository therefore provides **full documentation**, and the source code for our **memory game only**. Sorry for the inconvenience.

### Memory Game
In an effort to support cognitive health and dementia prevention, we wanted to stimulate short-term memory as well as cognitive sequencing, both of which are known to deteriorate in the early stages of dementia. This led us to develop this memory game, which generates random LED sequences for players to remember, then allows players to repeat the sequence using the corresponding buttons. Longer and longer sequences are displayed until failure. Our final product also included a timeout function, forcing users to respond quickly.

During our iterative development process, we worked towards building a game that was interactive, intuitive, and accessible for all. Take a look at our documentation for more details on the project.

## Documentation 📄
- [Emulator documentation](doc/interim_report.pdf) 
- [Assembler and Memory Game documentation](doc/final_report.pdf)

## Built With 🛠️
- **Languages**: C
- **Libraries**: GPIOD
- **Hardware**: Raspberry Pi 3, Breadboards, LEDs, Push Buttons, Resistors

## Usage 🚀
You will need the following hardware to play this game. We have implemented a scalable design; the number of buttons/LEDs can be changed easily by modifying the `NUM_BUTTONS` constant in the `src/memorygame_/game_state.h` file. By default, this number is 6.

### Prerequisites
- 1 x Raspberry Pi
- 2 x Breadboards
- 6 x Push Buttons
- 6 x LEDs
- 6 x Resistors (≥ 220Ω)
- ~25 x Jumper Wires

To set up the hardware, please refer to images [1](doc/hardware_pic.jpg) and [2](doc/hardware_pic_2.jpg)


### Instructions

1.  **Clone & Setup**

    Clone the repository and install the required GPIOD library on your Raspberry Pi.
    ```bash
    git clone https://github.com/rdesai-7/ARM64_MemoryGame.git
    cd ARM64_MemoryGame
    # Follow official instructions to install the GPIOD library
    ```

2.  **Configure (Optional)**

    Before compiling, you can navigate to the `src/memorygame_` directory and adjust game settings (like `NUM_BUTTONS` or GPIO pin assignments) in the `game_state.h` file. 

3.  **Compile & Run**

    Use `make` to compile the game. Run the executable and press the two outermost buttons simultaneously to begin playing!
    ```bash
    make
    ./memory_game
    ```

Scores are displayed in the terminal. We encourage adjusting the constants in `game_state.h` to change the difficulty and gameplay experience!

For any questions, please don't hesitate to email me at rohandesai706@gmail.com. Enjoy!

