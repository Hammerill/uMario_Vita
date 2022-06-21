# uMario Vita
Another uMario port for PSVita.

## Controls
- D-pad and left stick - controls at game and menu.
- Cross - jump at game and accept in menu.
- Square - run at game.
- Circle - dismiss at menu.
- Start and Select - enter menu at game and dismiss at menu.

## Known issues 
1. Sometimes some sounds are really VERY loud.
2. Sometimes player becomes invincible.
3. If you exit from or die in levels with fixed camera, camera will remain fixed until you exit the game (those three bugs might be legacy from original uMario_Jakowski).
4. Sometimes player spawns higher than they should (sometimes this allows player to climb at unavailable area right after spawn).
5. Due to wider screen you can see parts of levels that you normally shouldn't.

## Build and run
1. Install [VITASDK](https://vitasdk.org/).
2. Clone repo `git clone https://github.com/Hammerill/uMario_Vita && cd uMario_Vita`.
3. Build `cmake . && make`.
4. "uMario.vpk" file should appear. Copy this file to your hacked PS Vita and install via VitaShell.
5. Bubble "Mario" will appear, you can now launch this game.
