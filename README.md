# AR Madness

AR Madness (ARM) is a plugin editor and bus stop reporter (KaiLun BM-2000-KMB3) simulator for Midtown Madness 2 (MM2).

### Distribution Files

Distribution files are placed in **pack** directory. All compiled plugins are to be placed in **plugins** directory inside distribution directory.

### Programming Language

AR Madness is written with Visual Studio 2005 with MFC. Plugins are communicated via simulated SendMessage style messaging (Inspired from Miranda IM), so there are no hard library dependencies.

### Plugins

Apart from the main **ARM** project, the following plugins are available to ARM:

* **ARM-BSASLT**: The core to the BM-2000-KMB3 simulation. It provides I/O to other input and output plugins.
* **ARM-BSASConsole**: The console device simulator (UI) to BM-2000-KMB3. User commands are sent from this plugin to ARM-BSASLT.
* **ARM-Display**: The LED display simulator to BM-2000-KMB3. This plugin is currently hardcoded to 6 Full-width characters.
* **ARM-Sound**: The speaker interface to BM-2000-KMB3. It listens for sound event and plays specified wave / mp3 files.
* **ARM-BGKEY**: Listens for keyboard events when ARM is not in foreground, this is required to use ARM in MM2.
* **ARM-BGDisplay**: Hooks into Direct3D surface of MM2 and draw the LED and statistics.
* **ARM-ErrorCheck**: Checks for MM2 launch error and suggests possible conflict files (e.g. Multiple ar files patching the same file).
* **ARM-MM2GfxConf**: Change MM2 graphics configurations outside the game.
* **ARM-IMON**: SoundGraph iMON VFD support for display console contents are current English stop name.
* **ARM-OMSI**: PoC OMSI support, it works but with a side effect of content get reflected by side mirror.
