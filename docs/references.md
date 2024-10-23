# References

## Get devices

```{function} get_devices()

Returns a list of all devices connected to the Asgard Controllino.

:Return:
    `list[string]`: List of devices.
```

```python
import asgard_controllino as asgard

devices = asgard.get_devices()
print(devices) # ['DM1', 'DM2', 'DM3', 'DM4', '8893-K-M, 1+', '8893-K-M, 1-', ...]
```

---

## Class `Controllino`

```{function} Controllino(ip, port=23)

Create a new Controllino object.

:Parameters:
    **ip** `str`: IP address of the Controllino.
    
    **port** `int`, optional: Port of the Controllino. Default is 23.

:Return:
    `Controllino`: Controllino object.
```

```python
import asgard_controllino as asgard

ip = '172.16.8.200' # IP address you defined in the sketch
port = 23 # Port you defined in the sketch
controllino = asgard.Controllino(ip, port)
```

### Turn on/off

Control the power supply of the different components of the ASGARD platform.

```{admonition} Linked devices
:class: warning
Some devices cannot be powered at the same time. Make sure to turn off a device before turning on another one. Linked devices are:
- 8893-K-M, 1+ and 8893-K-M, 1-
- 8893-K-M, 2+ and 8893-K-M, 2-
- 8893-K-M, 3+ and 8893-K-M, 3-
- 8893-K-M, 4+ and 8893-K-M, 4-
```

```{function} .turn_on(device)

Turn on a device.

:Parameters:
    **device** `str`: Device to turn on.

:Return:
    `bool`: True if successful, False otherwise.
```

```python
controllino.turn_on('DM3')
```

```{function} .turn_off(device)

Turn off a device.  

:Parameters:
    **device** `str`: Device to turn off.

:Return:
    `bool`: True if successful, False otherwise.
```

```python
controllino.turn_off('DM1')
```

### Get status

```{function} get_status(device)

Get the status of a device.

:Parameters:
    **device** `str`: Device to get the status of.

:Return:
    `bool`: True if the device is on, False otherwise.
```

```python
powered = controllino.get_status('DM3')
print(powered) # True or False
```