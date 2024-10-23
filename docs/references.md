# References

## Devices

:::{function} get_devices()

Returns a list of all devices connected to the Asgard Controllino.

Available devices:
`MFF101/M 1-4`, `8893-K-M 1+`, `8893-K-M 1-`, `8893-K-M 2+`, `8893-K-M 2-`, `8893-K-M 3+`, `8893-K-M 3-`, `8893-K-M 4+`, `8893-K-M 4-`, `LS16P`, `DM1`, `DM2`, `DM3`, `DM4`, `XMC (BMX)`, `XMC (BMY)`, `XMC (BFO,SDL)`, `USB hubs both`, `MFF signal 1`, `MFF signal 2`, `MFF signal 3`, `MFF signal 4`

:Return:
    `list[string]`: List of devices.

:Example:
    ```python
    import asgard_controllino as asgard

    devices = asgard.get_devices()
    print(devices) # ['MFF101/M 1-4', '8893-K-M 1+', '8893-K-M 1-', ...]
    ```
:::

---

## Class `Controllino`

:::{function} Controllino(ip, port=23)

Create a new Controllino object.

:Parameters:
    **ip** `str`: IP address of the Controllino.
    
    **port** `int`, optional: Port of the Controllino. Default is 23.

:Return:
    `Controllino`: Controllino object.

:Example:
    ```python
    import asgard_controllino as asgard

    ip = '172.16.8.200' # IP address you defined in the sketch
    port = 23 # Port you defined in the sketch
    controllino = asgard.Controllino(ip, port)
    ```
:::

### Socket connexion

:::{attribute} .maintain_connexion

By default, each command open and close a connexion to the Controllino. This function allows to maintain the connexion open in order to reduce the time of each command. As a drawback, only the first established connection can communicate with the Controllino.
:Example:
    ```python
    # Open a unique connexion to turn off all the devices
    controllino.maintain_connexion = True
    for device in asgard.get_devices():
        controllino.turn_off(device)
    controllino.maintain_connexion = False
    ```
:::

### Device control

Control the power supply of the different components of the ASGARD platform.

:::{admonition} Linked devices
:class: warning
Some devices cannot be powered at the same time. Make sure to turn off a device before turning on another one. Linked devices are:
- 8893-K-M, 1+ and 8893-K-M, 1-
- 8893-K-M, 2+ and 8893-K-M, 2-
- 8893-K-M, 3+ and 8893-K-M, 3-
- 8893-K-M, 4+ and 8893-K-M, 4-
:::

:::{function} .turn_on(device)

Turn on a device.

:Parameter:
    **device** `str`: Device to turn on.

:Return:
    `bool`: True if successful, False otherwise.

:Example:
    ```python
    controllino.turn_on('DM3')
    ```
:::



:::{function} .turn_off(device)

Turn off a device.  

:Parameter:
    **device** `str`: Device to turn off.

:Return:
    `bool`: True if successful, False otherwise.

:Example:
    ```python
    controllino.turn_off('DM1')
    ```
:::

### Device status

:::{function} .get_status(device)

Get the status of a device.

:Parameter:
    **device** `str`: Device to get the status of.

:Return:
    `bool`: True if the device is on, False otherwise.

:Example:
    ```python
    powered = controllino.get_status('DM3')
    print(powered) # True or False
    ```
:::

### Piezo DAC

:::{function} .set_piezo_dac(value)

Set the value of the Piezo DAC.

:Parameter:
    **value** `float`: Value to set the Piezo DAC to. Must be between 0 (-> 0V) and 1 (-> 4.88V).

:Return:
    `bool`: True if successful, False otherwise.

:Example:
    ```python
    controllino.set_piezo_dac(0.5) # VB output of MCP4728 chipset should be 2.44V
    ```
:::