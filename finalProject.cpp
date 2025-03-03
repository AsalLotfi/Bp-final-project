// c++ program for implementation of Space Invaders game
#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;

// a function for color adjustment 
void setTextColor(int textColor, int bgColor = 0) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor | (bgColor << 4));
}

// a function to display flashing text
void blinkingText(const string &text, int blinkDelay = 500, int blinkCount = 10) 
{
    for (int i = 0; i < blinkCount; i++) 
    {
        // display flashing
        cout << "\033[5m" << text << "\033[0m" << endl; // restor flashing
        this_thread::sleep_for(milliseconds(blinkDelay)); // delay
        // clean the text
        cout << "\033[H\033[J"; // clean the screen 
        this_thread::sleep_for(milliseconds(blinkDelay)); // delay
    }
}

// a function for input the color and size of the board
void customizeBoard(int &width, int &height, int &color)
{
    cout << "Enter the width of the board: ";
    cin >> width;
    while (cin.fail() || width < 5) // check the width
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid width (min 5): ";
        cin >> width;
    }

    cout << "Enter the height of the board: ";
    cin >> height;
    while (cin.fail() || height < 5) // check the hight 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid height (min 5): ";
        cin >> height;
    }

    cout << "Enter the color code for the board (0-15): ";
    cin >> color;
    while (cin.fail() || color < 0 || color > 15) // check the input for 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a color code between 0 and 15: ";
        cin >> color;
    }
}

void drawBoard(int width, int height, int color)
{
    setTextColor(color);  // set the color board
    setTextColor(15);
}



// اAscii Art
void showAsciiArtMenu() 
{
    setTextColor(1); // text color
    cout << R"( 
 ███████╗██████╗  █████╗  ██████╗███████╗    ██╗███╗   ██╗██╗   ██╗ █████╗ ██████╗ ███████╗ ████████ ███████╗
 ██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝    ██║████╗  ██║██║   ██║██╔══██╗██╔══██╗██╔════╝ ██    ██ ██╔════╝ 
 ███████ ██████╔╝███████║██║     █████╗      ██║██╔██╗ ██║██║   ██║███████║██   ██╔█████╗   ████████ ███████╗  
 ══╝  ██╔██╔══╔╝ ██║  ██╔██     ║██║╚        ██║██║╚██╗ ██ ██   ██║██╔══██╝██╔══██ ██╔══╝   ██   ██       ██║
 ███████╗██║     ██║  ██║╚██████╗███████╗    ██║██║ ╚████║ ╚████╔╝ ██║  ██║██║███  ███████╗ ██    ██ ███████╗
 ╚══════╝╚═╝     ╚═╝  ╚═╝ ╚═════╝╚══════╝    ╚═╝╚═╝  ╚═══╝  ╚═══╝  ╚═╝  ╚═╝╚═╝     ╚══════╝ ╚═╝   ╚═╝╚══════╝
    )" << endl;

    setTextColor(4, 0); // turquoise color
    cout << R"(
╔═════════════════════════════════╗
║          MAIN MENU              ║
║═════════════════════════════════║
║[1] ➤ Start New Game             ║
║_________________________________║
║[2] ➤ Load Saved Game            ║
║_________________________________║
║[3] ➤ How to Play                ║
║_________________________________║
║[4] ➤ LeaderBoard                ║
║_________________________________║
║[5] ➤ Exit                       ║
╠═════════════════════════════════╣
)" << endl;

    setTextColor(15, 0); // restore the text to the default
}

// a function to show the explain about the game
void showGameInstructions() 
{
    setTextColor(11); //turquoise color
    cout << R"(

 
    ██████╗ ██╗  █████╗ ███   ███╗ ████████╗  ██╗    ██╗ ██╗ ██████╗  ███████╗
   ██╔═══██ ██║ ██╔══██╗╚██   ██╔  ██╔  ═══╝  ██║    ██║ ██║ ██╔══██╗ ██╔════╝
   ███████╗ ██║ ███████║ ███████║  ██   ████  ██║    ██║ ██║ ██   ██║ █████╗  
   ██║      ██║ ██╔══██║    ██║═╝  ██    ║██  ██║    ██║ ██║ ██   ██║ ██╔══╝  
   ██║      ██║ ██║  ██║    ██║    █████████╗  ███████║  ██║ ██████║  ███████
   ╚═╝      ╚═╝ ╚═╝  ╚═╝    ╚═╝     ╚═══════╝   ╚════╝   ╚═╝  ╚════╝   ╚═════╝

╔════════════════════════════════════════════════════════════════════════════════════════════════╗
║                                                                                                ║
║  Welcome to the space invaders! Here's how to play:                                            ║
║                                                                                                ║
║  ●1 object:                                                                                    ║
║      Protect your base from waves of enemy invaders.                                           ║
║      Destroy all invaders to progress to the next                                              ║
║      level. The game ends if the invaders reach your base                                      ║
║      or if your lives are depleted.                                                            ║
║  ●2 Controls:                                                                                  ║
║      Use the A Key and D Key to move your spaceship horizontally.                              ║
║      Press the Spacebar to shoot bullets upward.                                               ║
║      Use the P key to pause the game.                                                          ║
║   ●3 Gameplay:                                                                                 ║
║       Enemies: The invaders descend in rows.                                                   ║
║       Eliminate them before they reach the bottom of the screen.                               ║
║       Shields: Use shields to block enemy fire.                                                ║      
║       However, shields will degrade with repeated hits.                                        ║   
║       Scoring: Earn points for each invader destroyed.                                         ║
║       Special enemies may appear randomly for bonus points.                                    ║
║       Invaders Scores are as follows :                                                         ║
║       ⣟⣻ = 10 Score, ⢟⣛ = 20 Score, ⡋⢙ = 40 Score, ⢿⡿ = 100 Score.                             ║   
║       Lives: You start with a limited number of lives.                                         ║          
║       Avoid enemy projectiles to survive.                                                      ║     
║   ●4 Tips for win:                                                                             ║
║       Focus on the invaders closest to your base to                                            ║         
║       win the game faster. Watch for opportunities to shoot                                    ║
║       bonus enemies for extra points.                                                          ║
║       Use the shields strategically to block enemy's                                           ║
║       fire, but don’t rely on them too much.                                                   ║
║                                                                                                ║
║════════════════════════════════════════════════════════════════════════════════════════════════║
║                                                                                                ║
║               💡 TIP: Plan your moves carefully to maximize your score! 💡                     ║
║                                                                                                ║
║════════════════════════════════════════════════════════════════════════════════════════════════║
║                                                                                                ║
║                       *Press any key to return to the main menu...*                            ║
║                                                                                                ║
╚════════════════════════════════════════════════════════════════════════════════════════════════╝
    )" << endl;

    setTextColor(15); // restore the text to the default
    cin.ignore();
    cin.get();
}

// a function to show the second menu
void showSubMenu() 
{
    setTextColor(0, 4); // green text color
    cout << R"(
╔═══════════════════════════════════════╗
║             ★ NEW GAME ★              ║
║═══════════════════════════════════════║
║███████████████████████████████████████║
║█ [1] ➤ Quick Game                    █║
║███████████████████████████████████████║
║█ [2] ➤ Customize Game                █║
║███████████████████████████████████████║
║█ [3] ➤ BACK to MENU                  █║
║███████████████████████████████████████║
╚═══════════════════════════════════════╝
)" << endl;
    setTextColor(15, 0); // restore the text to the default
}
void animated_bold_red_text (const string& text, float delay = 0.1) 
{
    // ANSI code red and bold text
    const string bold_red = "\033[1;31m"; // bold and red
    const string reset = "\033[0m";       // restore the text to the default

    while (true) 
    {
        for (size_t i = 0; i < text.size(); ++i) 
        {
            // print text up to position 
            cout << bold_red << text.substr(0, i + 1) << reset << flush;
            this_thread::sleep_for(milliseconds(int(delay * 1000)));  // delay between displaying each character
            cout << "\r";  // restore the positon to start of the line 
            cout << string(text.size(), ' ');  // delete the last line 
            cout << "\r";  // restore to start of the line
            this_thread::sleep_for(milliseconds(int(delay * 1000)));  // delay after wiping
        }
    }
}


int main() 
{
    int choice;

    while (true) 
    {
        system("cls"); // clean the screen 

        showAsciiArtMenu(); // show the main menu 
        setTextColor(4);

        // get the user input
        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) 
        {
            int subChoice;

            while (true) 
            {
                system("cls"); // clean the screen 

                showSubMenu(); // show the second menu
                setTextColor(4);

                cout << "\nEnter your choice: ";
                cin >> subChoice;

                if (subChoice == 1) 
                {
                    cout << "Quick Game Selected!\n";
                } 
                else if (subChoice == 2) 
                {
                int width, height, color;
                system("cls");

                customizeBoard(width, height, color);  // get the size and color of the board
                drawBoard(width, height, color);  // show the new board

                cout << "\nPress any key to return to the sub-menu...";
                cin.ignore();
                cin.get();
                }
                else if (subChoice == 3) 
                {
                    break;
                } 
                else 
                {
                    cout << "Invalid choice! Please try again.\n";
                    Sleep(1000);
                }
            }
        } 
        else if (choice == 2) 
        {
            system("cls");

            string text = R"(
╔═══════════════════════════════════════╗
║            loading Game...            ║
╚═══════════════════════════════════════╝
                )";

            int blinkDelay = 500; // flashing time (ml)
            int blinkCount = 10;  // number of the flash

            setTextColor(11, 0);
            blinkingText(text, blinkDelay, blinkCount);

            cout << "\nPress any key to go back to the main menu...";
            cin.ignore();
            cin.get();
        } 
        else if (choice == 3) 
        {
            system("cls");
            showGameInstructions(); // show the explin of the board
        } 
        else if (choice == 4) 
        {
            // displayLeaderboard(); uncomment this section when the function is complete
        } 
        else if (choice == 5) 
        {
            system("cls");
            string text = "➤ GOOD BYE";
            animated_bold_red_text(text, 0.2);  // delay speed 0.2 sec

            break;
        } 
        else 
        {
            cout << "Invalid choice! Please try again.\n";
            Sleep(1000);
        }
    }

    return 0;
}
