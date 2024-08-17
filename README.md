<pre>
                   __      __            __      
  _______ ________/ / ___ / /  ___ _____/ /__ ___
 / __/ _ `/ __/ _  / (_-</ _ \/ _ `/ __/  '_/(_-<
 \__/\_,_/_/  \_,_/ /___/_//_/\_,_/_/ /_/\_\/___/
                                        
                  version 1.1
   Copyright (c) 2005-2024 Elwynor Technologies
</pre>

## WHAT IS CARD SHARKS?

 Card Sharks is a single-player card game that has the user try to
 gain as much money as possible by betting whether or not a card
 will be higher or lower than a displayed card.  A top ten
 list keeps the competition going.
 
## GAMEPLAY
 
 The object of Card Sharks is to finish with the largest amount of cash
 possible.  You begin with $250.00 and must go through 5 levels of betting.
 After each level you will recieve an additional $250.  The first four levels
 contain 5 cards and the last level has 2 cards.  On each level one card will
 be open for you to guess whether the next card will be higher or lower. Before
 the next card is revealed you must bet an amount less then or equal to what
 you have on hand.  If the next card is lower and you stated that it would be
 lower then you win the amount you bet.  If it is higher and you stated that
 it would be higher then you also win your bet.  But if the card is not in
 the direction that you stated, you LOSE!  If the card is the same then a PUSH
 occurs and no money is won or lost.  On the last level you must bet at least
 half your money before the last card is turned.  Also at the beginning of
 each level you will be asked if you want to change that card or not.  Don't
 forget that the ACE is the highest and the 2 is the lowest! 
 
## COMMANDS
 
 The in-game commands are simple - enter the amount to bet, and choose whether
 the next card will be higher or lower.  
 
 The following are main menu commands:
<pre>
 I  ...............  Read instructions
 P  ...............  Play Card Sharks
 T  ...............  Shows the Top Ten list
 X  ...............  Exits Card Sharks
</pre> 
 
## INSTALLATION AND CONFIGURATION
 
 Simply unzip the Card Sharks game archive to your BBS server directory,
 add Card Sharks to your menu, configure the MSG file to your liking, and start
 the BBS!  It's that easy! 
 
 Most of the configuration is text blocks in LEVEL6, but you can set the key 
 required to play in LEVEL3 and whether to show a log-on message in LEVEL4. 

 GAME HISTORY
 
 Card Sharks was originally written developed by Yannick Tessier in 1989 for 
 version 5 of The Major BBS.  The game was marketed by Tessier Technologies 
 (TTI), which became part of Galacticomm when TTI was merged into 
 Galacticomm, Inc in 1997, creating Galacticomm Technologies.
  
 Elwynor Technologies acquired the module in 2005, after converting it to 
 Major BBS 6/Worldgroup 1/Worldgroup 2 DOS in 2004. It was converted to 
 Worldgroup 3.2 in 2006, and Major BBS V10 in 2024. 
 
## LICENSE

 This project is licensed under the AGPL v3. Additional terms apply to 
 contributions and derivative projects. Please see the LICENSE file for 
 more details.

## CONTRIBUTING

 We welcome contributions from the community. By contributing, you agree to the
 terms outlined in the CONTRIBUTING file.

## CREATING A FORK

 If you create an entirely new project based on this work, it must be licensed 
 under the AGPL v3, assign all right, title, and interest, including all 
 copyrights, in and to your fork to Rick Hadsall and Elwynor Technologies, and 
 you must include the additional terms from the LICENSE file in your project's 
 LICENSE file.

## COMPILATION

 This is a Worldgroup 3.2 / Major BBS v10 module. It's compiled using Borland
 C/C++ 5.0 for Worldgroup 3.2. If you have a working Worldgroup 3.2 development
 kit, a simple "make -f ELWCARDS" should do it! For Major BBS v10, import this
 project folder in the isv/ subtree of Visual Studio 2022, right click the
 project name and choose build! When ready to build for "release", ensure you
 are building for release.
 
 This module also supports cross-compilation to the DOS-based Worldgroup 2
 platform. Often, DOS modules supported MBBS6/WG1/WG2 relatively consistently.
 The usual area of divergence was dependency upon GALME, which was introduced
 in WG2. In this case, during the WG3 transition, Galacticomm made a way for
 ISVs to cross-compile software to still support WG2 for DOS. The Makefile
 included here will compile under Borland C/C++ 4.52 for DOS. Use the mkv2p.bat
 file in the dev kit to do this: mkv2p ELWCARDS.

## PACKING UP

 The DIST folder includes all of the items that should be packaged up in a 
 ELWCARDS.ZIP. When unzipped in a Worldgroup 3.2 or Major BBS V10 installation 
 folder, it "installs" the module.

 The GALV3.DLL should not be included unless you're packaging for WG2 DOS.
 