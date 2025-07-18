# WFRP_Calculator

A command-line tool for managing the various currencies in **Warhammer Fantasy Roleplay 4th Edition (WFRP 4e)**.  
Written in C, this utility streamlines conversions between gold crowns, silver shillings, and brass pennies, making gameplay and bookkeeping easier for players and game masters.

## Features

- **Currency Conversion:** Easily convert between gold crowns, silver shillings, and brass pennies as used in WFRP 4e.
- **Simple CLI Usage:** Runs in the command line for quick access during games or planning.
- **Language Support:** Supports both **English** and **German** for user-facing messages.

## Usage

1. **Compile the Program**

   Use a C compiler to build the tool. Example with `gcc`:

   ```bash
   gcc -o wfrp_calculator wfrp_calculator.c -lm
   ```

2. **Run the Program**

   ```bash
   ./wfrp_calculator
   ```

   Follow the prompts to enter amounts and perform conversions.

## Language Selection

WFRP_Calculator supports **English** and **German** for all user messages.  
To set the language, edit the top of the `wfrp_calculator.c` source file and define **ONE** of the following preprocessor macros:

For **English**:
```c
#define LANG_EN
```

For **German**:
```c
#define LANG_DE
```

Make sure only one of these macros is defined at a time.  
After changing the macro, recompile the program to apply the language setting.

## Roadmap

- ESP32 hardware version planned, for portable and dedicated tabletop usage.
