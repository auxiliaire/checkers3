# checkers3

## The Chinese Checkers implementation in C / GTK+3

This application is a port and tailored version of [checkers2](https://github.com/auxiliaire/checkers2) which was written for GTK+2 as opposed to the current version featuring GTK+2

## Why Chinese Checkers?

Because it's fun and implementing it provides a good insight into a language/toolkit.

## How to build

See [Automake](https://www.gnu.org/software/automake/manual/automake.html)

* `./autogen.sh`
* `make`
* `make install` (if you're enthusiastic enough)

## Cross-platform

The app was written on Linux but it supports other platforms as well like Windows or OSX. See [Other OS Support](https://wiki.gnome.org/Projects/GTK+#Other_OS_support) for details.

## Porting a GTK+2 app to GTK+3

This project comparing with its predecessor [checkers2](https://github.com/auxiliaire/checkers2) can provide some hints about what needs to be changed in the code in order to make it work with GTK+3. Of course it's not a very sophisticated project so many gotchas necesseraly left out, but the basics seems to be covered.

## New features

The first version was written about ten years ago and never really finished (although working as a game). This project intended to upgrade the original source to be GTK+3 compatible and the appearance to be more Gnome3-ish. Therefore the button sidebar was eliminated and a HeaderBar was added featuring the only significant button. Application icon was also added.

## Screenshot

![Screenshot of the GTK+3 app in Gnome3](/checkers-gtk3.png)
