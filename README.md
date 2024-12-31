# Two-Dimensional Predator and Prey Simulation

## Overview
This project aims to create a basic two-dimensional predator-prey simulation using **object-oriented programming** principles, including classes, objects, inheritance, polymorphism, and encapsulation. The design facilitates the addition of new critters and allows the grid to be expanded to accommodate larger or more complex ecosystems.

In this simulation, the prey are ants, and the predators are doodlebugs. These critters live in a world composed of a 20 × 20 grid of cells. Only one critter may occupy a cell at a time. The grid is enclosed, so critters are not allowed to move off the edges of the world. Time is simulated in time steps, and each critter performs specific actions during each time step.

---

## Behavior Models

### Ant Behavior
1. **Move**:  
   - Every time step, an ant randomly attempts to move up, down, left, or right.  
   - If the chosen neighboring cell is occupied or outside the grid, the ant remains in its current cell.

2. **Breed**:  
   - If an ant survives for three time steps, it breeds at the end of the third step.  
   - Breeding creates a new ant in an adjacent (up, down, left, or right) empty cell.  
   - If no empty cell is available, breeding does not occur.  
   - After producing offspring, the ant must wait another three time steps before breeding again.


### Doodlebug Behavior
1. **Move**:  
   - Each time step, if there is an adjacent ant (up, down, left, or right), the doodlebug moves to that cell and eats the ant.  
   - If no adjacent ants are present, the doodlebug moves randomly, following the same rules as ants.  
   - Doodlebugs cannot eat other doodlebugs.

2. **Breed**:  
   - If a doodlebug survives for eight time steps, it breeds at the end of the eighth step.  
   - Breeding creates a new doodlebug in an adjacent (up, down, left, or right) empty cell.  
   - If no empty cell is available, breeding does not occur.

3. **Starve**:  
   - If a doodlebug does not eat an ant for three consecutive time steps, it starves and dies.  
   - The doodlebug is then removed from the grid.

---

## Simulation Rules
1. Start the simulation with 5 doodlebugs and 100 ants.
2. During each turn, **all doodlebugs move first**, followed by the ants.
3. The world is represented using ASCII characters:
  - `"o"` for ants
  - `"X"` for doodlebugs
  - `"-"` for empty cells

---

## Implementation Details
1. **Organism Class**:  
   - A base class called `Organism` encapsulates shared data and functionality for ants and doodlebugs.
   - This class includes a virtual function named `move` that is overridden in the derived classes `Ant` and `Doodlebug`.

2. **User Interaction**:  
   - After each time step, the program prompts the user to press `Enter` to proceed to the next step.

---

## Observations
Over time, the simulation should exhibit cyclical patterns in the populations of predators (doodlebugs) and prey (ants). However, random variations may occasionally lead to the extinction of one or both species.

---

## Example ASCII Output
```plaintext
----------------------
--ooo-----------------
---o-X----------------
--oo-Xoo--------------
----------------------
