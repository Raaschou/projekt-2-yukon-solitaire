Lige nu er der 5 mellemrum i mellem kolonner

Som udgangspunkt skriver vi GUI'en i SDL. Hvis vi altså 
kan få det til at fungere.
For at køre programmet på windows er løsningen blevet,
at installere SDL2 og SDL2_ttf igennem MSYS2, specifikt pakkerne:
mingw-w64-ucrt-x86_64-SDL2_ttf & mingw-w64-ucrt-x86_64-SDL2

Hvis man kører "pacman -S mingw-w64-ucrt-x86_64-SDL2_ttf" burde begge
blive installeret, plus andre SDL2_ttf dependencies.

$ProjectFileDir$ tryk på run edit configurations tilføj til woriking directory
Ellers kan man ikke loade fil.

1. [x] Source/Main.c
2. [x] Include/Main.h 
3. [x] Source/Deck.c
4. [x] Include/Deck.h
5. [x] Source/Game.c
6. [] Include/Game.h
7. [x] Source/Card.c
8. [ ] Include/Card.h
9. [] Source/Foundations.c
10. [ ] Include/Foundations.h
11. [ ] Source/FileIO.c
12. [ ] Include/FileIO.h
13. [] Source/Utils.c
14. [ ] Include/Utils.h
15. [x] Source/Board.c
16. [v] Source/LinkedLists.c
17. [ ] Include/LinkedLists.h
18. [x] Source/Columns.c
19. [ ] Include/Columns.h
20. [] Source/GUI.c
21. [ ] Include/GUI.h
22. [ ] Test/Test.c
23. [ ] Test/Test.h  
