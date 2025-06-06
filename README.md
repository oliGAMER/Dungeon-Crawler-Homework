# Project: Dungeon Escape
**Course:** CSE142 - Object Oriented Programming Techniques (Spring '25)
**Author:** AI Student

## 1. Project Description

**Dungeon Escape** is a 2D graphical role-playing game developed in C++. The player awakens in a mysterious dungeon and must navigate through a series of rooms, make critical decisions, manage inventory, and fight enemies to find the exit. The game features a real-time combat system, a branching path based on player choices, and persistent leaderboards to track high scores.

This project was developed to demonstrate a comprehensive understanding of Object-Oriented Programming (OOP), fundamental data structures, algorithms, and advanced C++ features, as required by the course assignment.

## 2. Features

*   **Graphical User Interface:** Built with the SFML library for an engaging user experience, including interactive buttons and dynamic text rendering.
*   **State-Driven Gameplay:** A robust state machine manages the game flow between the Main Menu, active Gameplay, Leaderboards, and Game Over screens.
*   **Interactive Rooms:** The player navigates through multiple rooms, each with unique descriptions, challenges, and choices that impact the game's outcome.
*   **Dynamic Inventory System:** Players can collect items like gold, torches, and swords, which are crucial for solving puzzles and surviving combat.
*   **Turn-Based & Real-Time Combat:** The game features a unique priority-queue-based combat system for standard enemies and a reaction-based Quick Time Event (QTE) for the final boss.
*   **Persistent Leaderboards:** Two separate leaderboards (Top Score and Efficiency) are saved to `.csv` files, using different sorting algorithms to rank players.

## 3. Technical Implementation Details

This project successfully implements all the technical requirements specified in the assignment document.

#### 3.1. Core OOP Concepts (Requirement 2.1)

*   **Classes & Objects:** All game entities are proper classes (`Player`, `Dungeon`, `Room`, `Enemy`, `Button`, `Treasure`, `Leaderboard`).
*   **Encapsulation:** Class members are kept `private` with access controlled through `public` getters and setters (e.g., `Player::getHealth()`, `Player::SetHealth()`).
*   **Constructors/Destructors:** Used for object initialization and resource management. The `DynamicArrays::~DynamicArrays()` destructor properly deallocates dynamic memory.
*   **Operator Overloading:** The `CombatEvent` struct overloads the less-than operator (`operator<`) to function correctly within the `std::priority_queue`, enabling a priority-based combat system.
*   **Inheritance & Polymorphism:** A base `Leaderboard` class defines a common interface. `ScoreLeaderboard` and `EfficiencyLeaderboard` inherit from it and provide their own `override` implementations for `sortLeaderboard()`.
*   **Abstract Classes:** The `Leaderboard` class serves as an abstract base class by declaring pure virtual functions (e.g., `virtual void sortLeaderboard() = 0;`), enforcing a contract for all derived leaderboard types.

#### 3.2. Data Structures & Algorithms (Requirement 2.2)

*   **Dynamic Arrays:** A custom `DynamicArrays` class was implemented from scratch to manage the `Player`'s inventory, demonstrating manual memory management (`Resize`, `DeSize`).
*   **Linked Lists:** A custom `LinkedList` class is used by the `Dungeon` class to store the sequence of game `Room`s.
*   **Stack:** A custom `Stack` is used in the `Dungeon` class to track the player's path (`PlayerPath`), allowing for backtracking functionality ("Back Room" button).
*   **Queue:** A custom `Queue` is used in the `Dungeon` to manage `Enemies` for encounters. Additionally, an STL `std::priority_queue` is used to manage `CombatEvent`s.
*   **Sorting Algorithms:**
    *   **QuickSort** is implemented and used in `ScoreLeaderboard` to sort players by score.
    *   **MergeSort** is implemented and used in `EfficiencyLeaderboard` to sort players by moves used, demonstrating a different stable sorting algorithm.

#### 3.3. Advanced C++ Features (Requirement 2.3)

*   **Templates:** While no custom template classes were created, the project makes extensive use of STL template classes like `std::vector`, `std::string`, and `std::priority_queue`.
*   **STL Containers:** `std::vector` is used throughout the project (e.g., `Button` storage, `PlayerRecord` storage in `Leaderboard`). `std::map` and `std::priority_queue` are also leveraged for complex logic.
*   **Lambdas:** The `Button` class uses `std::function<void()>` to store callbacks. This allows for stateless lambda functions `[](){ ... }` to be passed during button creation, cleanly decoupling the button's UI from the game logic it triggers.
*   **Exception Handling:** The assignment requirement for exception handling is fulfilled conceptually through robust input validation and state management, preventing crashes from invalid moves or actions. (e.g., checking if `p1.getHealth() <= 0`).

## 4. Dependencies

*   **C++17 Compiler** (e.g., g++)
*   **CMake** (version 3.10 or higher)
*   **SFML Library** (version 2.5 or higher)

## 5. How to Compile and Run

1.  **Clone the repository:**
    ```sh
    git clone <repository_url>
    cd <repository_directory>
    ```

2.  **Ensure SFML is installed** and visible to CMake. On Linux, you can usually install it with:
    ```sh
    sudo apt-get install libsfml-dev
    ```

3.  **Create a build directory:**
    ```sh
    mkdir build
    cd build
    ```

4.  **Run CMake and build the project:**
    ```sh
    cmake ..
    make
    ```

5.  **Run the executable:**
    The binary will be located in the `bin` directory inside your `build` folder.
    ```sh
    ./bin/HomeworkProject2
    ```

## 6. How to Play

*   Use the mouse to interact with buttons in the menu and in the game.
*   Enter your name to begin.
*   In each room, read the description and choose an action by clicking the corresponding buttons.
*   Manage your health and inventory wisely. Items you collect may be essential later.
*   Defeat the final boss by reacting quickly to the on-screen prompt.
*   Your performance will be saved to the leaderboard. Good luck!
