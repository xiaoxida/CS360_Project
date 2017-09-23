#include "Puzzle8Solver.h"
#include "Puzzle8PQ.h"
#include "Puzzle8StateManager.h"

using namespace std;

void WeightedAStar(std::string puzzle, double w, int & cost, int & expansions) {
	cost = 0;
	expansions = 0;

	// TODO
    Puzzle8StateManager myMannager;
    Puzzle8PQ openList;
    std::vector<std::pair<Puzzle8State, int>> myStates; //record the states
    
    Puzzle8State currState(puzzle);	// Create a state from its string representation.
    myStates.push_back(std::make_pair(currState, 0));
    myMannager.GenerateState(currState);

    PQElement startElement(0, GetF(0, currState.GetHeuristic(), w));
    openList.push(startElement);
    while(!openList.empty())
    {
        PQElement currElement = openList.top();
        openList.pop(); //remove current from open List
        myStates[currElement.id].first.SetClosed();   //add to closed list
        expansions++;
        currState = myStates[currElement.id].first;
        std::vector<Puzzle8State> currSuccessors = currState.GetSuccessors();
        for (Puzzle8State mySuccessor : currSuccessors)
        {
            if(IsGoal(mySuccessor.GetLinearizedForm()))
            {
                //we found the solution
                cost = myStates[currElement.id].second + 1;
                return;
            }
            if(!myMannager.IsGenerated(mySuccessor))  //if the successor did not go to openlist once
            {
                mySuccessor.SetOpened();
                myMannager.GenerateState(mySuccessor);
                int gVal = myStates[currElement.id].second + 1;
                myStates.push_back(std::make_pair(mySuccessor, gVal));
                int f = GetF(gVal, mySuccessor.GetHeuristic(), w);
                PQElement successorElement(myMannager.GetStateId(mySuccessor), f);
                openList.push(successorElement);
            }
            else    //if the successor has its id
            {
                int id = myMannager.GetStateId(mySuccessor);
                int gVal = myStates[currElement.id].second + 1;
                if(myStates[id].first.GetClosed())
                {
                    continue;
                }
                else if (myStates[id].first.GetOpened())  //if the successor is not closed and has its id, it must be in the open list
                {
                    int currF = GetF(gVal, mySuccessor.GetHeuristic(), w);
                    int preF = myStates[id].second;
                    if(currF < preF)
                    {
                        
                        myStates[id].second = currF;
                        PQElement successorElement(id, currF);
                        openList.push(successorElement);
                    }
                }
            }
        }
    }
}

double GetF(int g, int h, double w)
{
    return g + w * h;
}

bool IsGoal(std::string str)
{
    return (str == "012345678");
}

