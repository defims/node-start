# node-start

double click node-start = bash excute `contents\node contents\main.js`, both macosx and window support.

## usage

- download [files]()
- write your nodejs code start from `contents/main.js`

## build yourself

### windows

use [mingw32](http://www.mingw.org/)(gcc and windres)

## build node-start.o

    ```bash
    gcc src\node-start.c -c -o src\node-start.o
    ```

## build resouce (windres will be installed with mingw32)

if you want to change the icon, replace `src/icon.ico`

    ```bash
    windres src\node-start.rc -O coff -o src\node-start.res
    ```

## build node-start.exe

### show console

    ```bash
    gcc -o Release\windows\node-start src\node-start.o src\node-start.res
    ```

### hide console

    ```bash
    gcc -mwindows -o Release\windows\node-start src\node-start.o src\node-start.res
    ```

## put your code

download [node.exe](https://nodejs.org/en/download/), put it in `Release\windows\contents` and add your `main.js` file.

### warning: in main.js, the base path is `./` not `contents/`

## execute

double click `Release\windows\node-start.exe`, it will excute `contents\node.exe contents\main.js`

#macosx

use xcode built-in gcc

## show console

### build node-start unix excutable file

    ```bash
    gcc src/a.c -o Release/macosx/node-start
    ```

### change the icon

secondary click `node-start`, click `check info`, drag `src/icon.icns` into the icon.

### put your code

download [node binary](https://nodejs.org/en/download/), put it in `Release/macosx/contents` and add your `main.js` file.

### warning: in main.js, the base path is `./` not `contents/`

## execute

double click `Release/macosx/node-start`, it will excute `contents/node.exe contents/main.js`


## hide console

### coming soon
