# Raycaster README

## Overview
This project implements a basic 2D raycasting engine using **OpenGL** and **GLUT**. The engine generates a 3D projection of a simple map using the raycasting technique, commonly found in early 3D games like **Wolfenstein 3D**. It is designed to render a 2D top-down map and simulate a player's movement through a first-person perspective.

---

## Features
- **2D Top-Down Map Rendering**: Displays the map grid with walls and empty spaces.
- **3D Raycasting Visualization**: Projects 3D-like walls from the player's perspective.
- **Keyboard Controls**: Move and rotate the player in the virtual environment.
- **Dynamic Rendering**: Real-time rendering and updates based on player movement and direction.

---

## Prerequisites
### Software Requirements
- **OpenGL**
- **GLUT** (FreeGLUT recommended for compatibility)
- A C compiler (e.g., GCC)

### Platform
- Designed for **Linux**, **macOS**, and **Windows** systems supporting OpenGL.

---

## Installation and Usage
### Step 1: Install Required Libraries
Ensure you have OpenGL and GLUT installed. On most systems, you can install them with:
- **Ubuntu/Linux**:
  ```bash
  sudo apt-get install freeglut3-dev
  ```
- **macOS**:
  GLUT is included with the system's OpenGL framework.
- **Windows**:
  Install FreeGLUT from [http://freeglut.sourceforge.net/](http://freeglut.sourceforge.net/).

### Step 2: Compile the Program
Compile the program using `gcc` or your preferred C compiler:
```bash
gcc RaycasterEngine.c -o raycaster -lGL -lGLU -lglut -lm
```

### Step 3: Run the Program
Run the compiled executable:
```bash
./raycaster
```

---

## Controls
- **W**: Move forward
- **S**: Move backward
- **A**: Rotate left
- **D**: Rotate right

---

## Code Structure
### Key Components
1. **Map Representation**
   - The map is defined as a 16x16 grid of integers, where:
     - `1`: Wall
     - `0`: Empty space

2. **Player Movement**
   - The player's position (`px`, `py`) and direction (`pdx`, `pdy`) are dynamically updated based on keyboard input.

3. **Raycasting Logic**
   - Rays are cast from the player's position and checked for wall collisions in both horizontal and vertical directions.
   - Distance to the nearest wall is calculated, and a vertical slice is drawn on the screen to simulate 3D perspective.

4. **Rendering**
   - `drawMap2D()`: Renders the top-down map.
   - `drawPlayer2D()`: Renders the player's position and direction on the map.
   - `drawRays2D_3D()`: Performs raycasting and renders the 3D view.

---

## Customization
### Map Design
The `map` array can be modified to create custom layouts. Replace `1` with `0` or vice versa to create new wall configurations.

### Resolution
Adjust the screen resolution by modifying:
```c
#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080
```

### Performance
The number of rays can be modified for finer or coarser resolution:
```c
#define NUM_RAYS 1920
```

---

## Known Issues
- **Clipping**: If the player moves outside the map bounds, rendering behavior may be undefined.
- **Performance**: Rendering at very high resolutions or with a high number of rays may cause lag on older systems.

---

## Acknowledgments
- Inspired by classic raycasting techniques found in early 3D games.
- Utilizes OpenGL for rendering and GLUT for window management.

---

## License
This project is licensed under the MIT License. You are free to modify and distribute the code with proper attribution.
