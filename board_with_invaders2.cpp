#include <iostream>
#include <string>
#include <chrono>
#include <ctime> // for random seed 
#include <cstdlib> // for system("cls")
#include <unistd.h> // for sleep() function
#include <conio.h> // for _kbhit() and getch() functions
#include <windows.h>

using namespace std;
using namespace chrono;

steady_clock::time_point start_time = steady_clock::now(); 

// Define ANSI color codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"


// Game parameters
const int screen_height = 30;
const int screen_width = 30;
const int num_shelters = 4; // Number of shelters
// Dynamically calculate shelter dimensions
const int shelter_width = screen_width / 8; // Each shelter takes 1/8th of the board width
const int shelter_height = screen_height / 15; // Each shelter takes 1/15th of the board height
const int invadersCount = 20; // number of invaders
const int max_player_bullets = 10; // Define maximum bullets allowed on the screen
const int max_invader_bullets = 10; // Maximum number of bullets fired by invaders
const int max_health = 3; // all health bar
int player_health = max_health; // health bar in the moment
int saucer_direction = 1; // 1 for right, -1 for left
bool ready_to_shoot = true; // Flag to track if the player can shoot

// Unicode characters for Space Invaders style
const string INVADER_1 = "⡋⢙";      // invader type 1
const string INVADER_2 = "⢟⣛";      // invader type 2
const string INVADER_3 = "⣟⣻";      // invader type 3
const string FYLING_SAUCER = "⢿⡿";  // Red flying saucer
const string PLAYER_ICON = "⣾⣷";    // Player's icon
const string SHELTER = "⣿⣿";  // Shelter block

struct Cell 
{
    string entity;  // The type of entity (e.g., PLAYER, INVADER, SHELTER)
    string display; // The visual representation of the cell
    string color;   // The color of the entity
    int durability = 0; // Durability for shelters (default: 0)
};

// Structure to represent an Invader
struct Invader 
{
    int x, y;
    bool active = true; // to check whether the invader is active or not
};

// Structure to represent the player
struct Player
{
    int x; // Player's horizontal position
    string name;
    int score;
    double time;
};

// Structure to represent the flying saucer 
struct Saucer 
{
    int x = 0; // Saucer position
    bool active = false; // to check whether the saucer is active or not
};

// Structure to represent a bullet 
struct Bullet 
{
    int x, y;    // Bullet's position
    bool active; // Whether the bullet is active
};

int score = 0; // Initialize score to 0

Player player = {screen_width / 2}; // Initialize player in the center
Bullet player_bullets[max_player_bullets]; // Array to store bullets
Bullet invader_bullets[max_invader_bullets]; // Array to store invader bullets
Saucer saucer;

Cell game_board[screen_height][screen_width];

// Initialize invaders
Invader invaders[invadersCount] = 
{
    {4, 2, 1}, {8, 2, 1}, {12, 2, 1}, {16, 2, 1},
    {4, 4, 1}, {8, 4, 1}, {12, 4, 1}, {16, 4, 1}, 
    {4, 6, 1}, {8, 6, 1}, {12, 6, 1}, {16, 6, 1}, 
    {4, 8, 1}, {8, 8, 1}, {12, 8, 1}, {16, 8, 1}, 
    {4, 10, 1}, {8, 10, 1}, {12, 10, 1}, {16, 10, 1}
};

void clear_screen();
void playerShoot();
void display_status();
void initializePlayerBullets();
void movePlayerBullets();
void updateBoardWithPlayerBullets();
void place_shelters();
void board_maker(Invader invaders[], int invadersCount);
void invadersMoves(Invader invaders[], int invadersCount);
void playerMoves();
void moveSaucer();
void display_board();
void initializeInvaderBullets();
void moveInvaderBullets();
void invadersShoot();
void updateBoardWithInvaderBullets();
void end_game_menu();
void checkGameOverByInvaders(bool& gameOver);
void checkGameOverByPlayerHealth();
void hidecursor();



int main() 
{
    hidecursor();
    bool gameOver = false; // at first game is not over
    bool game_started = false; // Game starts paused until the player moves
    bool timer_started = false; // Flag to track if the timer has started

    // make the game board
    board_maker(invaders, invadersCount);
    initializePlayerBullets();
    initializeInvaderBullets();

    srand(time(0)); // Seed random number generator

    while (!gameOver)
    {
        // Handle player movement
        playerMoves();

        // check if the game should start
        if (!game_started)
        {
            // Start the game when the player makes the first move
            if (_kbhit()) 
            {
                char ch = getch();
                if (ch == 'a' || ch == 'd') 
                {
                    game_started = true; // Game starts
                    start_time = steady_clock::now();

                    if (ch == 'a')
                    {
                        player.x--;
                    }
                    else 
                    {
                        player.x++;
                    }
                }
            }

            // display the updated game board
            display_board();
            usleep(500000); // Delay for smoother visuals

            continue; // Skip the rest of the loop until the game starts
        }

        // Handle the invaders movement
        invadersMoves(invaders, invadersCount);
        moveSaucer();

        // Handle shooting and bullet movements
        playerShoot();
        movePlayerBullets();
        invadersShoot();
        moveInvaderBullets();

        // Update the game board
        board_maker(invaders, invadersCount);
        updateBoardWithPlayerBullets();
        updateBoardWithInvaderBullets();

        checkGameOverByPlayerHealth();

        // display the updated game board
        display_board();

        checkGameOverByInvaders(gameOver);

        if (gameOver)
        break;

        // Delay to control the game speed
        usleep(500000); // Sleep for 0.5 seconds 
    }

    return 0;
}



// a function to clear the screen on each frame
void clear_screen()
{
    system("cls");
    cout << "\033[2J\033[H"; // ANSI escape code to clear the screen
}

void end_game_menu()
{
    clear_screen();

    cout << RED << BOLD << "\nGAME OVER! TRY AGAIN...\n" << RESET;
    usleep(2000000); // 2 second delay


    // Display the menu
    cout << RED << BOLD << "╔";
    for (int i = 0; i < screen_width * 2; i++) 
    {
        cout << "═";
    }
    cout << "╗" << RESET << endl;

    cout << CYAN << BOLD << "║";
    cout << GREEN << "           GAME OVER           " << RESET;
    cout << CYAN << BOLD << "║" << RESET << endl;

    cout << CYAN << BOLD << "║";
    cout << YELLOW << "   1. Start again         " << RESET;
    cout << CYAN << BOLD << "║" << RESET << endl;

    cout << CYAN << BOLD << "║";
    cout << YELLOW << "   2. Save Game and Exit       " << RESET;
    cout << CYAN << BOLD << "║" << RESET << endl;

    cout << RED << BOLD << "╚";
    for (int i = 0; i < screen_width * 2; i++) 
    {
        cout << "═";
    }
    cout << "╝" << RESET << endl;

    // Handle the user's choice
    char choice;
    while (true) 
    {
        cout << GREEN << "Enter your choice (1 or 2): " << RESET;
        cin >> choice;

        if (choice == '1') 
        {
            // Restart the game
            score = 0; // Reset the score
            player_health = max_health; // Reset health
            player.x = screen_width / 2; // Reset player's position
            initializePlayerBullets(); // Clear bullets
            board_maker(invaders, invadersCount); // Reset the game board
            start_time = steady_clock::now(); // Reset the timer
            break; // Exit the menu and restart the game
        } 
        else if (choice == '2') 
        {
            // Save and exit
            cout << CYAN << "Saving the game...\n" << RESET;
            usleep(1000000); // Simulate saving delay
            exit(0); // Exit the game
        } 
        else 
        {
            cout << RED << "Invalid choice! Please enter 1 or 2.\n" << RESET;
        }
    }
}

// Initialize bullets
void initializePlayerBullets() 
{
    for (int i = 0; i < max_player_bullets; i++) 
    {
        player_bullets[i] = {-1, -1, false};
    }
}

// Function to shoot bullets
void playerShoot()  
{
    // Check if the player presses 'SPACE KEY' to shoot and no active bullet exists
    if (ready_to_shoot && _kbhit() && getch() == ' ') 
    {
        // Create a new bullet in the first inactive slot
        for (int i = 0; i < max_player_bullets; i++) 
        {
            if (!player_bullets[i].active) 
            {
                player_bullets[i] = {player.x, screen_height - 2, true};
                ready_to_shoot = false; // Disable shooting until the bullet is deactivated
                break;
            }
        }
    }
}

// a function to display the player's status (time, score, health)
void display_status()
{
    // if the time does not start, show the time = 0
    if (start_time.time_since_epoch().count() == 0)
    {
        cout << CYAN << BOLD << "Timer: " << 0 << "m " << 0 << "s   "
        << YELLOW << "Score: " << score << RESET;
    }
    else
    {
    // time elapsed since the start of the game
    steady_clock::time_point current_time = steady_clock::now();
    duration<int> elapsed_seconds = duration_cast<seconds>(current_time - start_time);  // converted to seconds

    int minutes = elapsed_seconds.count() / 60;  // calculate the min
    int seconds = elapsed_seconds.count() % 60;  // calculate the sec

    // show the time
    cout << CYAN << BOLD << "Timer: " << minutes << "m " << seconds << "s   "
    << YELLOW << "Score: " << score << RESET;
    }

    // show the Health Bar
    cout << RED << BOLD << "   Health: ";
    for (int i = 0; i < player_health; i++)
    {
        cout << "❤ "; // the charactor for health bar
    }
    cout << RESET << endl;
}

// Function to end the game once the invaders reached the shelters
void checkGameOverByInvaders(bool& gameOver)
{
    for (int i = 0; i < invadersCount; i++) 
    {
        if (invaders[i].y == screen_height - 8) // 8 ==> above the shelters position  
        {
            gameOver = true;
            cout << "Game Over! Invaders have reached the shelters!" << endl;
            cout << "Wait for the end game menu ...";
            usleep(5000000); // 5 second delay
            end_game_menu();
            return; // Exit early once game over is triggered
        }
    }
}

// a function to check the player collision
void checkGameOverByPlayerHealth()
{
    for (int i = 0; i < invadersCount; i++)
    {
        if (invaders[i].x == player.x && invaders[i].y == screen_height - 1)
        {
            player_health--; // reduce player's HB

            if (player_health > 0) 
            {
                for (int i = 0; i < 5; i++)
                {
                    clear_screen(); // Clear screen to display flashing
                    display_status(); // Show player status (health, scoreو timer)

                    if (i % 2 == 0) 
                    {
                        display_board(); // Display the game screen
                    }

                    usleep(400000); // 0.4 second delay
                }
            }

            if (player_health <= 0)
            {
                cout << RED << BOLD << "\nGame Over! You have no lives left." << RESET << endl;
                cout << "Wait for the end game menu ...";
                usleep(5000000); // 5 second delay
                end_game_menu();
                return; // end game 
            }
        }
    }
}

// Function to move bullets and handle collisions
void movePlayerBullets() 
{
    bool active_bullet_exists = false;

    for (int i = 0; i < max_player_bullets; i++) 
    {
        if (player_bullets[i].active) 
        {
            active_bullet_exists = true;

            // Move the bullet up
            player_bullets[i].y--;

            checkGameOverByPlayerHealth();

            // Deactivate the bullet if it goes off-screen
            if (player_bullets[i].y < 0) 
            {
                player_bullets[i].active = false;
            }

            // Check for collisions with shelters
            if (game_board[player_bullets[i].y][player_bullets[i].x].entity == "SHELTER") 
            {
                // Bullet hits a shelter block: "explode" the bullet
                player_bullets[i].active = false;
                continue;
            }

            // Check for collisions with invaders
            for (int j = 0; j < invadersCount; j++) 
            {
                if (invaders[j].x == player_bullets[i].x && invaders[j].y == player_bullets[i].y) 
                {
                    // Collision detected: deactivate bullet and invader
                    player_bullets[i].active = false;
                    invaders[j].active = false;
                    invaders[j].y = -40; // Remove invader from the board

                    // Update score based on the invader's row
                    if (invaders[j].y >= 28) // the lower row
                    score += 10;
                    else if (invaders[j].y >= 20) // the upper row
                    score += 20;
                    else if (invaders[j].y < 20) // the end row
                    score += 40;

                 break;
                }
            }
        }
    }

    // Allow shooting again if no active bullets remain
    if (!active_bullet_exists) 
    {
        ready_to_shoot = true;
    }
}

// Update the game board to include bullets
void updateBoardWithPlayerBullets() 
{
    for (int i = 0; i < max_player_bullets; i++) 
    {
        if (player_bullets[i].active && player_bullets[i].y >= 0 && player_bullets[i].y < screen_height) 
        {
            game_board[player_bullets[i].y][player_bullets[i].x] = {"BULLET", "| ", CYAN};
        }
    }
}

// Initialize invader bullets
void initializeInvaderBullets()
{
    for (int i = 0; i < max_invader_bullets; i++)
    {
        invader_bullets[i] = {-1, -1, false};
    }
}

// Function for invaders to shoot bullets
void invadersShoot()
{
    for (int i = 0; i < invadersCount; i++)
    {
        // Random chance for an invader to shoot
        if (invaders[i].y > 0 && rand() % 100 == 0) // 1% chance per tick
        {
            // Find an available slot for the bullet
            for (int j = 0; j < max_invader_bullets; j++)
            {
                if (!invader_bullets[j].active)
                {
                    invader_bullets[j] = {invaders[i].x, invaders[i].y + 1, true};
                    break;
                }
            }
        }
    }
}

// Function to move invader bullets and handle collisions
void moveInvaderBullets() 
{
    for (int i = 0; i < max_invader_bullets; i++) 
    {
        if (invader_bullets[i].active) 
        {
            // Move the bullet down
            invader_bullets[i].y++;

            // Deactivate the bullet if it goes off-screen
            if (invader_bullets[i].y >= screen_height) 
            {
                invader_bullets[i].active = false;
                continue;
            }

            // Check for collisions with shelters
            if (game_board[invader_bullets[i].y][invader_bullets[i].x].entity == "SHELTER") 
            {
                // Damage the shelter block
                Cell& shelter = game_board[invader_bullets[i].y][invader_bullets[i].x];
                shelter.durability--;

                // if durability reaches 0, remove the shelter block
                if (shelter.durability <= 0) 
                {
                    shelter = {" ", "  ", RESET, 0}; // Reset the cell to empty
                }

                // Deactivate the bullet
                invader_bullets[i].active = false;
                continue;
            }

            if (invader_bullets[i].y == screen_height - 1 && invader_bullets[i].x == player.x) 
            {
                // Bullet hits the player, reduce player's health
                player_health--;

                // Deactivate the bullet after it hits the player
                invader_bullets[i].active = false;

                // If player's health reaches 0, end the game
                if (player_health <= 0) 
                {
                    cout << RED << BOLD << "\nGame Over! You have no lives left." << RESET << endl;
                    exit(0); // end game 
                }

                break; 
            }
            
        }
    }
}

// Update the game board to include invader bullets
void updateBoardWithInvaderBullets()
{
    for (int i = 0; i < max_invader_bullets; i++)
    {
        if (invader_bullets[i].active && invader_bullets[i].y >= 0 && invader_bullets[i].y < screen_height)
        {
            game_board[invader_bullets[i].y][invader_bullets[i].x] = {"INVADER_BULLET", "| ", RED};
        }
    }
}

// Function to place shelters on the board
void place_shelters()
{
    const int spacing = (screen_width - (num_shelters * shelter_width)) / (num_shelters); // Spaces between shelters

    int start_y = screen_height - 7; // Vertical position for shelters (just above the player)

    for (int i = 0; i < num_shelters; ++i)
    {
        int start_x = spacing + i * (shelter_width + spacing) - 1; // Horizontal position for each shelter

        for (int y = 0; y < shelter_height; ++y)
        {
            for (int x = 0; x < shelter_width; ++x)
            {
                if (start_y + y < screen_height && start_x + x < screen_width)
                {
                    game_board[start_y + y][start_x + x] = {"SHELTER", SHELTER, GREEN, 3};
                }
            }
        }
    }
}

// a function to make the game board
void board_maker(Invader invaders[], int invadersCount) 
{
    // Initialize the board with empty cells
    for (int i = 0; i < screen_height; i++) 
    {
        for (int j = 0; j < screen_width ; j++) 
        {
            game_board[i][j] = {" ", "  ", RESET};
        }
    }

    // Place invaders on the board
    for (int i = 0; i < invadersCount; ++i) {
        if (invaders[i].y > 0 && invaders[i].y < screen_height && invaders[i].x > 0 && invaders[i].x < screen_width) 
        {
            if (i < 4)
                game_board[invaders[i].y][invaders[i].x] = {"I1", INVADER_1, MAGENTA};
            else if (i < 8)
                game_board[invaders[i].y][invaders[i].x] = {"I2", INVADER_2, YELLOW};
            else if (i < 12)
                game_board[invaders[i].y][invaders[i].x] = {"I3", INVADER_2, YELLOW};
            else if (i < 16)
                game_board[invaders[i].y][invaders[i].x] = {"I3", INVADER_3, BLUE};
            else 
                game_board[invaders[i].y][invaders[i].x] = {"I3", INVADER_3, BLUE};
        }
    }

    // Set player position
    if (player.x > 0 && player.x < screen_width)
    {
        game_board[screen_height - 1][player.x] = {"PLAYER", PLAYER_ICON, GREEN};
    }

    // Set flying saucer position
    if (saucer.active && saucer.x >= 0 && saucer.x < screen_width) 
    {
        game_board[0][saucer.x] = {"SAUCER", FYLING_SAUCER, RED};
    }

    // Place shelters on the board
    place_shelters();
}

// a function to move the invaders
void invadersMoves(Invader invaders[], int invadersCount)
{
    static int direction = 1; // initial direction: Right 
    bool shouldChangeDirection = false;

    // Check for boundary collisions
    for (int i = 0; i < invadersCount; i++)
    {
        if (invaders[i].x + direction >= screen_width || invaders[i].x + direction <= 0)
        {
            shouldChangeDirection = true;
            break; // Direction change is needed, no need to check further
        }
    }

    // Change direction if needed
    if (shouldChangeDirection)
    {
        direction = -direction; // Reverse direction
        for (int i = 0; i < invadersCount; i++)
        {
            invaders[i].y += 1; // Descend after changing direction
            
            if (invaders[i].y >= screen_height - 1) 
            {
                invaders[i].y = screen_height - 2; // Prevent invaders from exceeding the screen height
            }
        }
    }

    // Move the invaders 
    for (int i = 0; i < invadersCount; i++)
    {
        invaders[i].x += direction;
    }
}

// a function to move the player's ship
void playerMoves()
{
    if (_kbhit()) // Check if a key is pressed
    {
        char ch = getch();
        if (ch == 'a' && player.x > 0) 
        {
            player.x--; // Move left
        } 
        else if (ch == 'd' && player.x < screen_width - 1) 
        {
            player.x++; // Move right
        }
    }
}

// a function to move the flying saucer
void moveSaucer() 
{
    static int spawn_chance = 100; // Approximate probability (higher = rarer) \ in this case is %1

    // If saucer is not active, decide randomly if it should appear
    if (!saucer.active) 
    {
        if (rand() % spawn_chance == 0) 
        { // Randomly activate saucer
            saucer.active = true;

            // Set initial position
            if (saucer_direction == 1)
            {
                saucer.x = 0;
            }
            else 
            {
                saucer.x = screen_width - 1;
            }
        }
        return;
    }

    // Move the active saucer
    saucer.x += saucer_direction;

    // If the saucer exits the screen, deactivate it
    if (saucer.x < 0 || saucer.x >= screen_width )
    {
        saucer.active = false;
    }
}

// a function to display the game board
void display_board() 
{
    clear_screen();

    display_status();

    // Display top border
    cout << MAGENTA << BOLD << "╔";
    for (int i = 0; i < screen_width * 2; i++) 
    {
        cout << "═";
    }
    cout << "╗" << RESET << endl;

    for (int i = 0; i < screen_height; i++) 
    {
        cout << CYAN << BOLD << "║" << RESET; // Left border
        for (int j = 0; j < screen_width; j++) {
            cout << game_board[i][j].color << BOLD << game_board[i][j].display << RESET;
        }
        cout << CYAN << BOLD << "║" << RESET << endl; // Right border
    }

    // Display bottom border
    cout << MAGENTA << BOLD << "╚";
    for (int i = 0; i < screen_width * 2; i++) 
    {
        cout << "═";
    }
    cout << "╝" << RESET << endl;
}

void hidecursor() 
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}
