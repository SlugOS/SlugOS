# Extra flags you can pass

## Disabling Drivers
You can pass these to EXTRA_DEFINES in make.config

```NO_PCI``` This function will disable PCI and its drivers (this will stop all of the PCI drivers too)

```DEBUG``` This function will put in extra testing for stuff like testing PCI (will conflict with NO_PCI)