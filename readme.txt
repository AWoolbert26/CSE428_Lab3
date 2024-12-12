Lab 3
Daniel Tymoshenko d.tymoshenko@wustl.edu
Rohan Tatikonda r.s.tatikonda@wustl.edu
Andrew Woolbert a.s.woolbert@wustl.edu

Problems:

     - Iterator Syntax Errors: Misunderstanding how to properly implement and use begin() and end() iterators in the CardSet class template.

     - Errors in defining or passing predicates for the collect_if method, leading to incorrect filtering or collection of cards.

     - Overlooking edge cases when using std::move and std::back_inserter, potentially leading to dangling references or unintentional data retention.

     - Duplicate Player Names: Overlooking checks for duplicate names during player initialization, potentially causing confusing game states.

     - Turn Input Validation: Mishandling invalid player numbers or card ranks during input, leading to crashes or undefined behavior.

     - Empty Deck Handling: Incorrectly managing game states when the deck is empty, such as failing to properly eliminate players or terminate the game.

     - Game Termination Logic: Failing to handle all conditions for ending the game, such as when only one player remains or all books are collected.

     - Command-Line Argument Parsing: Overlooking edge cases for invalid or improperly formatted arguments, leading to unhelpful error messages.


Testing:

-Testing for different command line arguments:
    Note that I'm using IDE which displays the exit code differently that the usual terminal would do.
    1) No arguments: executing ./lab3 gives us:
        Usage: ./lab3 <GameType> <PlayerNames...>
        GameType: Pinochle <Player1> <Player2> <Player3> <Player4>
         or HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
         or GoFish <DeckType> <Player1> <Player2> [<Player3> ... <Player9>]
            (Exit code 7)
        
        That's a proper usage message describing two games implemented and number of players we need to specify for each of them. 

    2) Only one argument specifying wrong type of game: ./lab3 Poker
        Unknown game type: Poker
        Usage: ./lab3 <game_name> <PlayerNames...> 
        (Exit code 10)

        Proper usage message telling that there's no such game.

    3) Wrong game name and some players specified: ./lab3 Poker Bob Alica
        Unknown game type: Poker
        Usage: ./lab3 <game_name> <PlayerNames...>
        (Exit code 10)

        The exit code is also 10 meaning wrong game type argument.

    4) Pinochle game, no players: ./lab3 Pinochle
        Wrong number of players, Pinochle requires exactly four players.
        Usage: ./lab3 Pinochle <Player1> <Player2> <Player3> <Player4>
        (Exit code 8)

        The error code is different this time and the mesage displays that the problem is in the number of players.

    5) Pinochle game, not enough players: ./lab3 Pinochle 1 2 3
        Wrong number of players, Pinochle requires exactly four players.
        Usage: ./lab3 Pinochle <Player1> <Player2> <Player3> <Player4>
        (Exit code 8)

        Again the error is the same as in 4) which is correct.

    6) Pinochle game, too many players: ./lab3 Pinochle 1 2 3 4 5
        Wrong number of players, Pinochle requires exactly four players.
        Usage: ./lab3 Pinochle <Player1> <Player2> <Player3> <Player4>
        (Exit code 8)

        Again the error is the same as in 4) which is correct.
    
    7) Pinochle game, correct number of players: ./lab3 Pinochle 1 2 3 4
        1's hand:
        QD KH AS 9D 10H JC QS 9H AH JD 10S 10C 
        pinochle 40

        2's hand:
        KC JD QH AH 10C 9S AS JH 10D AC KS QC 

        3's hand:
        AD 9S QH KD KS JS KH JS JH AC 10S QS 

        4's hand:
        9C 10H QD 10D AD QC JC 9H KC KD 9D 9C 


        Do you want to end the game? (yes or no): 

        The game started successfully which is a correct behavior. 

    8) Pinochle game, correct number of players but two of gthem have the same name: ./lab3 Pinochle 1 1 3 4
        In Pinochle, players can't have the same names.
        All player names should be unique.

        (Exit code 13)

        The output is correct, no same names are allowed in Pinochle.

   
    9) HoldEm game, no players provided: ./lab3 HoldEm
        Wrong number of players, HoldEm requires from 2 to 9 players.
        Usage: ./lab3 HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
        (Exit code 9)

        The message is correct and the exit code is different from when Pinochle game didn't have the right amount of players. 
        This is a correct behavior because HoldEm is a separate game which has different exectuion rules, so it should have a unique command line error code.

    10) HoldEm game, not enought players provided: ./lab3 HoldEm Bob
        Wrong number of players, HoldEm requires from 2 to 9 players.
        Usage: ./lab3 HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
        (Exit code 9)

        We got the same error which is correct.

    11) HoldEm game, too many players provided: ./lab3 HoldEm Bob1 Bob2 Bob3 Bob4 Bob5 Bob6 Bob7 Bob8 Bob9 Alex
        Wrong number of players, HoldEm requires from 2 to 9 players.
        Usage: ./lab3 HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
        (Exit code 9)

        Again that's a correct behavior since we provided a wrong number of players.

    12) HoldEm game, nine players provided, which is correct: ./lab3 HoldEm Bob1 Bob2 Bob3 Bob4 Bob5 Bob6 Bob7 Bob8 Bob9
        Bob1's hand:
        3H 4H 
        Bob2's hand:
        AD 2D 
        Bob3's hand:
        AS 10S 
        Bob4's hand:
        8C 10D 
        Bob5's hand:
        4D KC 
        Bob6's hand:
        JS 5C 
        Bob7's hand:
        2C 9H 
        Bob8's hand:
        KS QC 
        Bob9's hand:
        JH 6H 
        BOARD (flop): 
        JC 5S 2H 
        Bob6 JS 5C 2H 5S JC 
         twopair
        Bob9 JH 6H 2H 5S JC 
         pair
        Bob2 AD 2D 2H 5S JC 
         pair
        Bob7 2C 9H 2H 5S JC 
         pair
        Bob3 AS 10S 2H 5S JC 
         xhigh
        Bob8 KS QC 2H 5S JC 
         xhigh
        Bob5 4D KC 2H 5S JC 
         xhigh
        Bob4 8C 10D 2H 5S JC 
         xhigh
        Bob1 3H 4H 2H 5S JC 
         xhigh
        BOARD (turn): 
        JC 5S 2H 4C 
        BOARD (river): 
        JC 5S 2H 4C 3C 

        Do you want to end the game? (yes or no):


        Again we get proper behavior as in the previous lab.

    13) HoldEm game, only two players provided, which is also correct: ./lab3 HoldEm Bob1 Bob2
        Bob1's hand:
        7D JS 
        Bob2's hand:
        9C 8H 
        BOARD (flop): 
        8C 9S 2S 
        Bob2 9C 8H 2S 9S 8C 
         twopair
        Bob1 7D JS 2S 9S 8C 
         xhigh
        BOARD (turn): 
        8C 9S 2S 2D 
        BOARD (river): 
        8C 9S 2S 2D AD 

        Do you want to end the game? (yes or no): 

        Again the game started successfully, which is right.

    14) HoldEm game, a correct number of players is provided but they two of them are have the same name: ./lab3 HoldEm Bob 1 2 Bob
        In HoldEm, players can't have the same names.
        All player names should be unique.

        (Exit code 13)

        The error is treated correctly.

    15) 