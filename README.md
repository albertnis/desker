# Desker

*Controlling desk lighting with Arduino*

In April 2017 , I puchased some SK6812 individually-addressable LED strips. After cutting and soldering them to size, I fastened them at various strategic locations around my desk: beneath speakers, beneath, monitors, and behind monitors. With [backlight][] controlling my existing 12V strip behind my desk, I decided I needed a new, simpler solution.

Desker is a continuous work-in-progress which I will update as I discover useful functionality or hardware which would increase my desk lighting experience. I started desker from scratch to remove a lot of the bloat which backlight had gathered over the last few years. For example, desker has a small range of preset colours based on the colours I actually used from backlight. Gone is colour detection and music synchronisation, both awesome features but features which were rarely used and which do not suit to down-to-earth goals of desker.

### Features

- Selectable colours using potentiometers.
- Compatability with SK6812and WS8212B LED strips (or any Neopixel-supported strip).
- Zone mapping - in my case behind desk (12V strip), behind each screen, and desktop lighting.

### Future Features

I am looking forward to adding in some functionality from backlight:

- Crossfading
- IR remote control

There are also tons of new features I may add:

- White colour temperature varying throughout day/night (I have a DS3231 just for this!).
- Behind-monitor light based on screen content.

[backlight]: https://github.com/albertnis/backlight