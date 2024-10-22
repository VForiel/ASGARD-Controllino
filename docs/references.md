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

### `Turn on/off`

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
