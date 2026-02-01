I worked on Windows. 

I used the instructions provided to help guide me through implementation. While writing, I would look through other header files to know what functions and classes I can use. There was a lot of trial and error and testing to make the red squiggle line go away. 

For the AI, the negamax function recursively goes through each game state to check for a winner, draw and returns bestVal. The negamax function also uses aiBoardEval to get the winning triples and returns winner if there is a winning triple.

UpdateAI gets the state of the board using stateString and uses negamax to get a score. If the score is better than the current score, replace it with the new score. Then, if a valid move is found, execute it. 
