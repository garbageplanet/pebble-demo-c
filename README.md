## Garbagepla.net Pebble app demo
This is the app that sits on your pebble and talks to its [android companion app](https://github.com/garbageplanet/android) to push markers to the garbagepla.net api.

## Geting started

Go to [Cloudpebble](https://cloudpebble.net/ide/), create a new project and add the `main.c` and `main.h` files, build the project and launch it either in the emulator as APLITE / BASALT or to your watch with the developer connection.

## TODO

- [ ] confirm with phone it recorded the marker properly before notify user of success
- [ ] add UI elements: up is cancel and down is send to phone
- [ ] automatically send to phone after shake and delay ?
- [ ] detect shaking / tap either as simple tap or with both acceleration directions
- [ ] ...