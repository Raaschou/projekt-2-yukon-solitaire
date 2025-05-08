Som udgangspunkt skriver vi GUI'en i SDL. Hvis vi altså 
kan få det til at fungere.
For at køre programmet på windows er løsningen blevet,
at installere SDL2 og SDL2_ttf igennem MSYS2, specifikt pakkerne:
mingw-w64-ucrt-x86_64-SDL2_ttf & mingw-w64-ucrt-x86_64-SDL2

Hvis man kører "pacman -S mingw-w64-ucrt-x86_64-SDL2_ttf" burde begge
blive installeret, plus andre SDL2_ttf dependencies.

$ProjectFileDir$ tryk på run edit configurations tilføj til woriking directory
Ellers kan man ikke loade fil.

Denne applikation bruger **DejaVu Sans** til tekstvisning i SDL GUI.

Fontfil: `DejaVuSans.ttf`  
Placering: `asset/DejaVuSans.ttf`

### Licens

DejaVu Sans er open source og distribueres under følgende licens:
[Bitstream Vera Fonts Copyright](https://dejavu-fonts.github.io/License.html)

> "Permission is hereby granted, free of charge, to any person obtaining a copy of the fonts..."

### Kilde

Fonten er hentet fra det officielle DejaVu Fonts-projekt:  
👉 https://dejavu-fonts.github.io/

For 

Terminal er case unsensitve. udover det følger den meget godt opgaven.
Funktioner :
StartUpPhase-
Load deck : LD
Sace deck : SD
Load Game : L
Play      : P
Split Shuffle : SI
Random Shuffle : SR
Quit : QQ
Show deck : SW

PlayPhase -
Redo : R
Undo : U
Quit : Q
Save game : S
Moves : fx x->y, x:kort->y

I Gui er det hele enten en knap eller trykke på et kort.
Det hvide fungere som et anuller tryk, hvis man har valgt et kort

For at kunne give input (fil navn) i GUI skal man trykke på den grå boks, 
når den er aktiveret forstår GUI at der kommer en command og et argument boksen bliver i øvrigt rød,
fx til load deck, save deck, load game og save game