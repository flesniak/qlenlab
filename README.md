qlenlab
=======

Ein alternatives Interface, das die Standard-API der Messhardware “LENlab v3.1” des IBT am KIT (www.ibt.kit.edu) implementiert. Läuft unter Linux und Mac OS, ein Windows-Port ist gerade in Entwicklung. Kommt im Gegensatz zur Original-Software ohne MATLAB aus, baut auf qt4, qwt und fftw auf.

Kompilieren (Unix)
------------------

Um qlenlab zu kompilieren werden qt4, qwt6 und fftw benötigt. In manchen Fällen (wenn Qt5 und Qt4 installiert ist) muss der Aufruf von `qmake` durch `qmake-qt4` ersetzt werden.

```sh
$ mkdir build
$ cd build
$ qmake ..
$ make
```

An diesem Punkt lässt sich die Datei `qlenlab` direkt ausführen, man kann qlenlab aber auch direkt installieren (aus dem Ordner `build`):

```sh
$ sudo make install
```

Das Installationsverzeichnis wird nicht wie üblich mit der `DESTDIR`-Variable gesetzt sondern mit `INSTALL_ROOT`.
