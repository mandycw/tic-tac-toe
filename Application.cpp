#include "Application.h"
#include "imgui/imgui.h"
#include "classes/TicTacToe.h"
#include <fstream>
#include "logger.h"
using namespace std;

namespace ClassGame {
        //
        // our global variables
        //
        TicTacToe *game = nullptr;
        bool gameOver = false;
        int gameWinner = -1;

        //
        // game starting point
        // this is called by the main render loop in main.cpp
        //
        void GameStartUp() 
        {
            game = new TicTacToe();
            game->setUpBoard();
        }

        //
        // game render loop
        // this is called by the main render loop in main.cpp
        //
        void RenderGame() 
        {
                ImGui::DockSpaceOverViewport();
                 //log window
                Logger::getInstance().renderLogWindow();
                //ImGui::ShowDemoWindow();

                if (!game) return;
                if (!game->getCurrentPlayer()) return;
                
                ImGui::Begin("Settings");
                ImGui::Text("Current Player Number: %d", game->getCurrentPlayer()->playerNumber());
                ImGui::Text("Current Board State: %s", game->stateString().c_str());

                if(game->checkForDraw() == false && game->checkForWinner() == nullptr){
                    if(ImGui::Button("AI")){
                        
                    }

                    ImGui::SameLine();

                    if(ImGui::Button("Save Game")){
                        std::ofstream gameSave("save.txt");
                        if(gameSave){ 
                        gameSave << game->stateString();
                        Logger::getInstance().LogInfo("Game State Saved. Game State: " + game->stateString());
                        gameSave.close();
                    } else {
                        Logger::getInstance().LogError("Could Not Save Game State.");
                    }
                }

                    ImGui::SameLine();

                    if(ImGui::Button("Load Game")){
                        std::ifstream gameLoad("save.txt");
                        if(gameLoad){ 
                            std::string gameState;
                            gameLoad >> gameState;
                            game->setStateString(gameState);
                            Logger::getInstance().LogInfo("Saved Game State Loaded. Game State: " + game->stateString());
                        } else {
                            Logger::getInstance().LogError("Save File Not Found.");
                        }
                    }

                    ImGui::SameLine();

                    if(ImGui::Button("Reset")){

                        game->stopGame();
                        Logger::getInstance().LogInfo("Game Was Reset.");
                    }
                    


                }



                if (gameOver) {
                    ImGui::Text("Game Over!");
                    ImGui::Text("Winner: %d", gameWinner);
                    if (ImGui::Button("Reset Game")) {
                        game->stopGame();
                        game->setUpBoard();
                        gameOver = false;
                        gameWinner = -1;
                    }
                }
                ImGui::End();

                ImGui::Begin("GameWindow");
                game->drawFrame();
                ImGui::End();

    

            

        }

        //
        // end turn is called by the game code at the end of each turn
        // this is where we check for a winner
        //
        void EndOfTurn() 
        {
            Player *winner = game->checkForWinner();
            if (winner)
            {
                gameOver = true;
                gameWinner = winner->playerNumber();
            }
            if (game->checkForDraw()) {
                gameOver = true;
                gameWinner = -1;
            }
        }
}
