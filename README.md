# Map Editor

A 2D RPG-style map editor written in C++ using the SFML multimedia library.

Requires SFML-2.5 or thereabouts.

It's setup to be built in CodeBlocks IDE, just setup the correct Search Directories under Build Options. You could compile it in literally any other IDE or environment if you want. It should run on Linux, although the case of some filenames might be an issue?

It's not the most user friendly thing in the world, and the map format leaves a bit to be desired, but it's quite usable.

Controls:
- Select tile from basic tileset with `[1]` to `[9]` keys.
- Zoom with the scroll wheel. 
- Arrow keys to move. 
- Left click to place tile.
- Right click to remove tile
- `[Esc]` to return to the main menu where you can load, save, quit, etc.

Licensed under the terms of the MIT license _with the **exception** of_ the images in `graphics/` and the fonts in `fonts/`.