# Pong Game

This is a simple implementation of the classic Pong game with an automated adversary paddle, that uses a basic ray cast algorithm to predict the ball's next position.

## Prerequisites

Before compiling and running this code, make sure you have the `SDL` library installed, you can install it on Debian-based distributions by running the following command:

```
sudo apt-get install libsdl2-dev
```

## Compilation

To compile the code, make sure you have `make` installed. In the source directory run the following command:

```
make
```

## Usage

To play the game, run the `pong` executable after compilation:

```
./pong
```

## Cleanup

To remove the generated files, you can simply use the following command:

```
make clean
```
