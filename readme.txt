Lab 3
Daniel Tymoshenko d.tymoshenko@wustl.edu
Rohan Tatikonda r.s.tatikonda@wustl.edu
Andrew Woolbert a.s.woolbert@wustl.edu

Methodology and Process:

    Steps 1-6: Copied lab2 into the repository and renamed everything that needed it. Fixed a few small errors from lab2 such as the Ace low straight being the lowest
               and three of a kind not checking for high card.
    
    Step 7: Added the begin and end accessors for the cards member variable of cardset. No errors here. 

    Step 8: Added a sort method to the CardSet class template. Refactored play in both pinochle and holdem games to use the iterators. 
            Did the same for holdem_hand_evaluation and suit_independent_evaluation. No Errors or issues

    Step 9: Added type specifiers for the card type, suit type, rank type, and iterator type in the public portion of the declaration of the CardSet class template. No errors or issues.

    Step 10: Moved the collect method from the Deck class template up into the CardSet class template, and rewrote it to use the std::move function. No errors or issues.

    Step 11: Added the collect_if method to the CardSet class template. No errors or issues.

    Step 12: Added the request method to the CardSet class template. Made the lab and it compiled and ran the same as in lab2. No errors or issues. 

    Step 13: Created UnoDeck header and source files. Added a color enumeration. No errors or issues.

    Step 14: Declared the UnoRank enumeration, its operators, and the same oporators for Color. No errors or issues. 

    Step 15: Declared and defined the UnoDeck class. No Errors or issues.

    Step 16: Declared and defined the GoFishGame class template. I did decide to use count_if to validate different decks and created a helper function. checkIfValidDeck to do so.
             I did run into a number of errors while coding this section. At first I was unsure how to parameterize the templates but eventually, through trial and error, found 
             an effective configuration. I decided to make the GoFishGame class inherit from the Game class as that allows it to be created in the create helper function which is called
             in the main function. Because I did have it inherit publically from main we needed to pass argc and argv into the constructor in order to pass those along to the game base constructor.
             Without argc and argv Game would be unable to construct the correct player_names list for GoFishGame as the player names start one later than HoldEm and Pinochle.

    Step 17: Added the static assert statement into the template header. No errors or issues. 

    Step 18: Added the collect books method to the GoFishGame class template. Simply used a count if a


Testing:

-Testing for different command line arguments:
    Note that I'm using IDE which displays the exit code differently that the usual terminal would do.
    1) No arguments: executing ./lab2 gives us:
        Usage: ./lab2 <GameType> <PlayerNames...>
        GameType: Pinochle <Player1> <Player2> <Player3> <Player4>
            or HoldEm <Player1> <Player2> [<Player3> ... <Player9>] 
            or GoFish <DeckType> <Player1> <Player2> [<Player3> ... <Player9>]
            (Exit code 7)
        
        That's a proper usage message describing two games implemented and number of players we need to specify for each of them. 

    2) Only one argument specifying wrong type of game: ./lab2 Poker
        Unknown game type: Poker
        Usage: ./lab3 <game_name> <PlayerNames...> 
        (Exit code 10)

        Proper usage message telling that there's no such game.

    3) Wrong game name and some players specified: ./lab2 Poker Bob Alica
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

    8) Pinochle game, correct number of players but two of gthem have the same name: ./lab2 Pinochle 1 1 3 4
        1's hand:
        QD 9H QH 10C AC QC AH 9S QC KH KC JD 

        1's hand:
        KH QS QS 10D 10S 9H 9C 10H AS 10D 10C KD 

        3's hand:
        9C JH 10H QD KD JS AC KS 9D JD JC JS 
        fortyjacks 40

        4's hand:
        AS 10S JH KS 9S KC AH QH AD JC 9D AD 

        BUG! The game cannot doesn't really make sense when some players have absolutely the same names.
        I added an additional check for that into both Pinochle and HoldEm games.
        Implementation strategy: usually to check if an array has two equal elements we would sort it or use set/map, but here since the number of players is at most 9 a regular nested for range loop will be really fast and will avoid unnecessary library #includes
        Also we implement this check on the initialization stages for both games instead of implementing it for the whole Game class, because, in theory, some games might allow having players with the same name (for example a game might have 2 players but they play in some specific order which is specified in this way).

        Let's test again: ./lab2 Pinochle 1 1 3 4
        In Pinochle, players can't have the same names.
        All player names should be unique.

        (Exit code 13)

        Now the output is correct.

    9) Since we added additional check for players let's see again if the game start correctly if all names are unique: ./lab2 Pinochle 1 2 3 4
        1's hand:
        QC KD QH JD 9D 10C JS JC KH 10D 10H 10C 

        2's hand:
        9C 9S QS 9D AH 10H QC 9H AC JC AH QD 

        3's hand:
        9H JS AC 9S JH KC KD KH 9C AS AS 10S 

        4's hand:
        JD 10D KS KC JH QH AD KS QS QD AD 10S 
        pinochle 40


        Do you want to end the game? (yes or no): 

        The game started correctly.

    10) HoldEm game, no players provided: ./lab2 HoldEm
        Wrong number of players, HoldEm requires from 2 to 9 players.
        Usage: ./lab2 HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
        (Exit code 9)

        The message is correct and the exit code is different from when Pinochle game didn't have the right amount of players. 
        This is a correct behavior because HoldEm is a separate game which has different exectuion rules, so it should have a unique command line error code.

    11) HoldEm game, not enought players provided: ./lab2 HoldEm Bob
        Wrong number of players, HoldEm requires from 2 to 9 players.
        Usage: ./lab2 HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
        (Exit code 9)

        We got the same error which is correct.

    12) HoldEm game, too many players provided: ./lab2 HoldEm Bob1 Bob2 Bob3 Bob4 Bob5 Bob6 Bob7 Bob8 Bob9 Alex
        Wrong number of players, HoldEm requires from 2 to 9 players.
        Usage: ./lab2 HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
        (Exit code 9)

        Again that's a correct behavior since we provided a wrong number of players.

    13) HoldEm game, nine players provided, which is correct: ./lab2 HoldEm Bob1 Bob2 Bob3 Bob4 Bob5 Bob6 Bob7 Bob8 Bob9
        Bob1's hand:
        QS 5H 
        Bob2's hand:
        AS 8C 
        Bob3's hand:
        4H 9H 
        Bob4's hand:
        6S KH 
        Bob5's hand:
        2C 10S 
        Bob6's hand:
        10C AC 
        Bob7's hand:
        JH 5C 
        Bob8's hand:
        AH 6C 
        Bob9's hand:
        KD 8D 
        BOARD (flop): 
        10D 2H JC 
        Bob5 2C 10S JC 2H 10D 
        twopair
        Bob7 JH 5C JC 2H 10D 
        pair
        Bob6 10C AC JC 2H 10D 
        pair
        Bob2 AS 8C JC 2H 10D 
        xhigh
        Bob8 AH 6C JC 2H 10D 
        xhigh
        Bob9 KD 8D JC 2H 10D 
        xhigh
        Bob4 6S KH JC 2H 10D 
        xhigh
        Bob1 QS 5H JC 2H 10D 
        xhigh
        Bob3 4H 9H JC 2H 10D 
        xhigh
        BOARD (turn): 
        10D 2H JC 7H 
        BOARD (river): 
        10D 2H JC 7H 3S 

        Do you want to end the game? (yes or no):

        The game started successfully which is totally right! This passes cases 2 and 3 in step 19 of the lab. the correct combination of cards is produced for each player with everyone having
        the three cards from the common board in addition to their own two unique card and the list of ranked hands being displayed correctly. They are printed from highest ranked to lowest with
        two pair winning, the pairs beneath, and then the high cards listed below those. Even thi high cards are listed in correct order!

    14) HoldEm game, only two players provided, which is also correct: ./lab2 HoldEm Bob1 Bob2
        Bob1's hand:
        JC 9S 
        Bob2's hand:
        2H QD 
        BOARD (flop): 
        5D AC 8H 
        Bob2 2H QD 8H AC 5D 
        xhigh
        Bob1 JC 9S 8H AC 5D 
        xhigh
        BOARD (turn): 
        5D AC 8H QS 
        BOARD (river): 
        5D AC 8H QS JH 

        Do you want to end the game? (yes or no): 

        Again the game started successfully, which is right.

    15) HoldEm game, a correct number of players is provided but they two of them are have the same name: ./lab2 HoldEm Bob 1 2 Bob
        In HoldEm, players can't have the same names.
        All player names should be unique.

        (Exit code 13)

        The error is treated correctly.
