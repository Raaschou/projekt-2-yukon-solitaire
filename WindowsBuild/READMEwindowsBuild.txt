Windows Build Guide:

1. Åbn projektet i CLion på Windows
2. Byg med standard CMake (Debug)
3. Kopiér følgende til cmake-build-debug:
   - SDL2.dll
   - SDL2_ttf.dll
   - /Kort/bmp/
   - /font/din_font.ttf
   - optional: easy_win_deck.txt
4. Dobbeltklik på exe-filen

De medfølgende SDL2.dll og SDL2_ttf.dll stammer fra de officielle builds udgivet af SDL-projektet.
Begge biblioteker er udgivet under zlib-licensen:
DLL-filerne (SDL2.dll og SDL2_ttf.dll) er hentet direkte fra de officielle udviklerpakker fra SDL-projektet.
De stammer fra:
SDL2-devel-2.28.5-VC.zip
SDL2_ttf-devel-2.20.2-VC.zip
Disse pakker er beregnet til Windows og inkluderer de nødvendige .dll-filer til runtime.
Vi har kun inkluderet de to nødvendige DLL’er i mappen, så spillet kan køre uden yderligere installation.