
Debian
====================
This directory contains files used to package polcoind/polcoin-qt
for Debian-based Linux systems. If you compile polcoind/polcoin-qt yourself, there are some useful files here.

## polcoin: URI support ##


polcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install polcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your polcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/polcoin128.png` to `/usr/share/pixmaps`

polcoin-qt.protocol (KDE)

