# uMario Vita
Another uMario port to PS Vita.

## Controls
- D-pad and left stick - controls at game and menu.
- Cross - jump at game and accept in menu.
- Square - run at game.
- Circle - dismiss at menu.
- Start and Select - enter menu at game and dismiss at menu.

## Known issues 
1. Sometimes some sounds are really VERY loud.
2. Sometimes player becomes invincible (those two bugs might be legacy from uMario_Jakowski).
3. Enemies spawn higher than they should.
4. High, inaccurate main menu.
5. No button rebinding (should I really fix this? If you would, you can use some plugins for this).
6. Due to wider screen you can see parts of levels that you normally shouldn't.

## Build and run
1. Install [VITASDK](https://vitasdk.org/).
2. Clone repo `git clone https://github.com/Hammerill/uMario_Vita && cd uMario_Vita`.
3. Build `cmake . && make`.
4. "uMario.vpk" file should appear. Copy this file to your hacked PS Vita and install via VitaShell.
5. Bubble "uMario" will appear, you can now launch this game.
