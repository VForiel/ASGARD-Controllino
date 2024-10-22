ASGARD-Controllino
==================

This python library allows you to control the power supply of the different components of the ASGARD platform.

# 🔌 Installation

1. **Setup the controllino**

    Edit the first lines of the following sketch to put the correct MAC adresse and the desired IP address.

    👉 [sketch.ino](https://github.com/VForiel/ASGARD-Controllino/blob/Test-dynamic-pin/sketch/sketch.ino)

    Then upload the sketch to the controllino. The sketch might require the "Controllino" library the you can find on the Arduino IDE.

2. **Connect your computer to the same local network as the controllino**

3. **Install the python library** (soon)

    ```bash
    pip install asgard-controllino
    ```

# 🚀 Quickstart

```python
from asgard_controllino import Controllino

ip = '172.16.8.200' # IP address you defined in the sketch
controllino = Controllino(ip) 
controllino.turn_on('DM3')
controllino.turn_off('DM1')
```

```{toctree}
:hidden:
:maxdepth: 1

references
```