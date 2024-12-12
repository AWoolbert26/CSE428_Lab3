Lab 3
Daniel Tymoshenko d.tymoshenko@wustl.edu
Rohan Tatikonda r.s.tatikonda@wustl.edu
Andrew Woolbert a.s.woolbert@wustl.edu

Problems:

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

    Step 18: Added the collect books method to the GoFishGame class template. Simply used a count if to check if there are 4 cards of a certain rank and then collect_if to transfer it to 
             the players books set. No errors or Issues.

    Step 19: Building out the turn method for the GoFishGame class template took, by far, the most time. I made sure that the hand output is nicely compacted so that the user can see  
             all players hands at the same time. The prompting of the user for a valid card rank and player number was a little complicated. I solved this by adding a from_string method
             to all of the Deck classes. This method takes in a string and, if it is valid, returns a corresponding rank. This allowed me to check if the string that the player submits
             is actually equal to the rank of one of the cards in his hand. I decided to just transfer one card over instead of many cards. I also implemented the gofish method if the 
             request is unsuccessful. If the players hand is empty They automatically GoFish.

    Step 20: Implemented the deal method. No errors or issues. 

    Step 21: Implemented the play method. No errors or issues. 

    Step 22: Changed the create method to check if the game is GoFish and create a GoFishGame specialized for different deck types. Also changed Game so that if the game is GoFish it creates
             the player_names vector starting one later. No errors or issues.

    Step 23: Modified the programs main function to accept GoFish and uses the create function accordingly. No errors or issues.

    Step 24: When I made the entire thing there were numerous errors and issues throughout the codebase the compiler wasn't able to detect them until I actually instantiated a GoFishGame instance.
             these took a very long time to sort out ...

    Step 25: Ran numerous tests as described below.


Testing:

-Testing for different command line arguments:
    Please note that I'm using IDE which displays the exit code differently that the usual terminal would do.
    1) No arguments: executing ./lab3 gives us:
        Usage: ./lab3 <GameType> <PlayerNames...>
        GameType: Pinochle <Player1> <Player2> <Player3> <Player4>
         or HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
         or GoFish <DeckType> <Player1> <Player2> [<Player3> ... <Player9>]
        (Exit code 7)
        
        That's a proper usage message describing two games implemented and number of players we need to specify for each of them. 

    2) Only one argument specifying wrong type of game: ./lab3 Poke
        Unknown game type: Poke
        Usage: ./lab3 <game_name> <PlayerNames...> 
        (Exit code 10)

        Proper usage message telling that there's no such game.

    3) Wrong game name and some players specified: ./lab3 Poker Michael Peter
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

    8) Pinochle game, correct number of players but two of them have the same name: ./lab3 Pinochle 1 1 3 4
        In Pinochle, players can't have the same names.
        All player names should be unique.

        (Exit code 13)

        The output is correct, no same names are allowed in Pinochle.


    9) Pinochle game, correct arguments provided, we will test playing game: ./lab3 Pinochle 1 2 3 4
   
        1's hand:
        QS 10C KC 10H KH AS JS QH JC QC KD KS 
        eightykings 80

        2's hand:
        9C 9S AD AH KC AD AS AC JH JC 9D 10D 
        hundredaces 100

        3's hand:
        KD QD KS QS AH JH AC KH QH 10S 9C QD 

        4's hand:
        JD 10H JS 9D 10C 10S 9H JD QC 9S 10D 9H 

        Do you want to end the game? (yes or no):

        Here in first hand eighty kings combo was correctly detected as the hundreadaces one in the second had.
        The output is proper for pinochle, we continue playing the game by saying "no".

        Do you want to end the game? (yes or no): no
        1's hand:
        KS 10H QD QD JS KD JC JH 10S 10C QH AD 

        2's hand:
        QS AS 10H AC AC 9C 9H KS 10S JH KH 9S 

        3's hand:
        KD QC AD 10D 9S JC JD AH 9D 10C 10D AS 

        4's hand:
        QS JD KH 9C KC JS KC QH AH 9H 9D QC 
        pinochle 40

        Do you want to end the game? (yes or no): 

        Here there's only was combination which is in 4's hand and it was correctly recognized. The score assigned to the pinochle combo is correct. We continue playing. 

        1's hand:
        JH 10S KS AS JS AC 9H AD QS QD 9D AS 

        2's hand:
        10C KC JD 9C AD 10H QC 9D QH QC 10D AH 

        3's hand:
        10S KD KH KD JC JS QD JC JH QH KC 9C 

        4's hand:
        10H 9S AH KH QS 9S 9H AC JD KS 10D 10C 
        pinochle 40

        Do you want to end the game? (yes or no): 

        Again 1,2,3's hands don't have any combos, so there's nothing to find in there. 4's still has pinochle, which is right.

        I have made around 10 more turns in this game and haven't run into any issue which is a good sign that no new bugs have arised since the second lab.


    10) HoldEm game, no players provided: ./lab3 HoldEm
        Wrong number of players, HoldEm requires from 2 to 9 players.
        Usage: ./lab3 HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
        (Exit code 9)

        The message is correct and the exit code is different from when Pinochle game didn't have the right amount of players. 
        This is a correct behavior because HoldEm is a separate game which has different exectuion rules, so it should have a unique command line error code.

    11) HoldEm game, not enought players provided: ./lab3 HoldEm Bob
        Wrong number of players, HoldEm requires from 2 to 9 players.
        Usage: ./lab3 HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
        (Exit code 9)

        We got the same error which is correct.

    12) HoldEm game, too many players provided: ./lab3 HoldEm Bob1 Bob2 Bob3 Bob4 Bob5 Bob6 Bob7 Bob8 Bob9 Alex
        Wrong number of players, HoldEm requires from 2 to 9 players.
        Usage: ./lab3 HoldEm <Player1> <Player2> [<Player3> ... <Player9>]
        (Exit code 9)

        Again that's a correct behavior since we provided a wrong number of players.

    13) HoldEm game, nine players provided, which is correct: ./lab3 HoldEm Bob1 Bob2 Bob3 Bob4 Bob5 Bob6 Bob7 Bob8 Bob9
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

    14) HoldEm game, only two players provided, which is also correct: ./lab3 HoldEm Bob1 Bob2
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

    15) HoldEm game, a correct number of players is provided but they two of them are have the same name: ./lab3 HoldEm Bob 1 2 Bob
        In HoldEm, players can't have the same names.
        All player names should be unique.

        (Exit code 13)

        The error is treated correctly.

    16) GoFish game with no arguments provided: ./lab3 GoFish
        Wrong number of players, GoFish requires from 2 to 5 players.
        Usage: ./lab3 GoFish <DeckType> <Player1> <Player2> [<Player3> ... <Player5>]

        (Exit code 9)

        That's a proper error showing how to start GoFish game.

    17) GoFish game with Uno deck and two players: ./lab3 GoFish UnoDeck 0 1

        Current Game State:
        It is Player 0's turn:
        Player 0 (0):
          Hand: 8Blue 6Blue WildBlack Draw FourBlack 8Red 2Blue 4Blue
          Books:

        Player 1 (1):
          Hand: 9Blue ReverseGreen 0Green 5Blue 4Red ReverseGreen 5Blue
          Books:

        Enter a rank (e.g., zero, one, two, skip, etc.):

        We try entering something that is not a card, like "notacard", and get:

        You don't have any cards of that rank or the card type you entered doesn't exist. Try again.

        This is a proper responce. Now let's try entering card type that we(player 0) don't own, for example 7. We get:

        You don't have any cards of that rank or the card type you entered doesn't exist. Try again.

        That's again right. Then finally let's do a proper move. Let's choose 2, which another player doesn't have.

        Enter a rank (e.g., zero, one, two, skip, etc.): 2
        Enter a player number (e.g., 0, 1, 2, 3, etc.): 1
        Current Game State:
        It is Player 1's turn:
        Player 0 (0):
          Hand: 8Blue 6Blue WildBlack Draw FourBlack 8Red 2Blue 4Blue 9Blue
          Books:

        Player 1 (1):
          Hand: 9Blue ReverseGreen 0Green 5Blue 4Red ReverseGreen 5Blue
          Books:


        As we see since player 1 doesn't have 2 we drawed a card from the deck and received 9Blue which is correct. One might get confused by why there player 1 also has card 9Blue and player 0 now has received another 9Blue card, but as described in step 15 of the lab manual, there are two 9Blue cards in the Uno deck.

        Now we are player #1 and let's choose a card that we don't have.

        Enter a rank (e.g., zero, one, two, skip, etc.): 2
        You don't have any cards of that rank. Try again.

        Again we got a correct behavior.

        Now let's finally steal a card that player 0 has in their deck, which is 9Blue.

        Enter a rank (e.g., zero, one, two, skip, etc.): 9
        Enter a player number (e.g., 0, 1, 2, 3, etc.): 0
        Current Game State:
        It is Player 1's turn:
        Player 0 (0):
          Hand: 8Blue 6Blue WildBlack Draw FourBlack 8Red 2Blue 4Blue
          Books:

        Player 1 (1):
          Hand: 9Blue ReverseGreen 0Green 5Blue 4Red ReverseGreen 5Blue 9Blue
          Books:


        We successfully stole the 9Blue card from the player 0's deck and added it at the end of ours, which is correct!

        Now we are asked again to make a turn for player 1, which is correct as our turn was successful so we get to make another one. I did around 20 more casual turns testing this game out and didn't run into any issues.

    18) We did the same deep testing for GoFish game based on the Pinochle deck and didn't run into any problems. Let's show some important steps from this testing.

        Another player has a few card of the requested type, but only one is drawn as described in the lab manual step 16:

        Current Game State:
        It is Player 1's turn:
        Player 0 (0):
          Hand: JC KS QD QD 9C JD QH 9S 9D 10D
          Books:

        Player 1 (1):
          Hand: QH QS KH JC 10S
          Books:

        Enter a rank (e.g., zero, one, two, skip, etc.): Q
        Enter a player number (e.g., 0, 1, 2, 3, etc.): 0
        Current Game State:
        It is Player 1's turn:
        Player 0 (0):
          Hand: JC KS QD 9C JD QH 9S 9D 10D
          Books:

        Player 1 (1):
          Hand: QH QS KH JC 10S QD
          Books:

          Exactly how it's needed. After that we reqest Queen again to get a book:

          Enter a rank (e.g., zero, one, two, skip, etc.): Q
        Enter a player number (e.g., 0, 1, 2, 3, etc.): 0
        Current Game State:
        It is Player 1's turn:
        Player 0 (0):
          Hand: JC KS 9C JD QH 9S 9D 10D
          Books:

        Player 1 (1):
          Hand: KH JC 10S
          Books: QH QS QD QD

        The book was recognized correctly and game continues to work properly on all test cases.

    19) The game ended correctly and output a correct message.