#include <duels/snake/msg.h>
#include <duels/server.h>
#include <duels/snake/snake_ai.h>
#include <duels/snake/mechanics.h>

using namespace duels::snake;
using duels::Result;
using duels::Timeout;
using duels::Refresh;
using GameIO = duels::Server<InitDisplay, Input, Feedback, Display>;


int main(int argc, char** argv)
{
    GameIO game_io("snake", Timeout(100), Refresh(15));

    // simulation time
    [[maybe_unused]] const double dt(game_io.samplingTime());

    // TODO prepare game state / init message (for display)
    SnakeMechanics mechanics;
    InitDisplay init = mechanics.initGame();

    /* Added part to prepare the game state */

    Display display = mechanics.display();


    /* ========== Adding the parameters for the initial game state ========== */

    // The images are 16 by 16 pixels and we want to create a 720 by 512 window

    // We add 3 apples to the game
    while(gameState.apples.size() < 3)
    {mechanics.newApple(gameState.apples);}
    init.apples = gameState.apples;


    // inform displays and get players (multithread by default for simultaneous games)
    const auto [player1, player2] = game_io.initPlayers<SnakeAI>(argc, argv, init, 0, 1); {}
    display.apples = init.apples;


    while(true)
    {
        // extract feedbacks
        const auto result = mechanics.buildPlayerFeedbacks(player1->feedback, player2->feedback, init.width, init.height, gameState);
        // stop if game over
        if(result != Result::NONE)
        {
            game_io.endsWith(result);
            game_io.sendDisplay(mechanics.display());
            break;
        }

        // TODO build display information
        // Need to send the new positions of the snakes and the apples.

//        display.head1 = gameState.snake1.head;
//        display.body1 = gameState.snake1.body;
//        display.head2 = gameState.snake2.head;
//        display.body2 = gameState.snake2.body;
//        // Each time an apple is ate by a Snake, it will automatically refill one.
//        display.apples = gameState.apples;

        game_io.sendDisplay(mechanics.display());


        // request player actions, exits if any disconnect / crash
        if(!game_io.syncBothPlayers())
            break;

        // TODO update game state from player1->input and player2->input
        mechanics.update(player1->input, player2->input);


    }

    // final results
    game_io.sendResult(mechanics.display());
}
