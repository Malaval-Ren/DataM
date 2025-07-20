
# **DataM**

![Alt text](./MM_PowerUp_256x256.png "DataM")    ![Alt text](./gplv3-127x51.png "license") 

_Creation: June 13th, 2024..2025, by Renaud Malaval_  
_Last review: July 20th, 2025, by Renaud Malaval_

## Convert many stuff

Just a little software to help **Apple IIGS** cross development

Convert a list of strings in a .txt file to a .bin file with at beginning index table to a string all stored after.  
Line begin by "/* " are ignored.  
Empty line are ignored.

## License

This software is under the **GPL v3** license

## Build

Open the project file **datam.sln** with **Visual Studio Community 2022** and build it

## Help usage

```bash
datam.exe v1.11.12.29, Copyright (c) 2024..2025 R. Malaval & F. Mure

Usage: datam <option> "<filespec>" <convmspec> "<number>" "<filespec>" "<output folder>"

  <option> is one of the following
   +lower       - the output file name is in lower case
   +chkenu      - Check synchro enum eSentence_t and list of string
                  <filespec> the path to file with enum eSentence_t definition

  <convmspec> is one of the following
   -dump        - dump content of a supported file format
   -sltb        - file with strings list to binary file
   -ctbl        - create table from .cvs to .c
   -getstr      - get string at index in binary file

  <filespec> file extension could be :
   -dump          'any'

  <filespec> :
   -sltb          'pathname to txt file'
  <output folder> :
   -sltb          'pathname to folder destination'

  <number> could be:
   -getstr        '0 .. 999'
  <filespec> :
   -getstr        'pathname to bin file'
```

## Sample usage

```bash
datam -v
datam -?

datam +LOwer +chkEnu pathname.h -SLTB datatext.fr.txt ..\dessin\
datam +chkenu pathname.h -sltb datatext.fr.txt ..\dessin\
datam -sltb datatext.fr.txt ..\dessin\

datam -getstr 400 datatext.fr.bin
datam -getstr 400 datatext.en.bin
```

>note:
>character '…' is converted to character '/' to be compliant with the picture where the char '/' is a '…'

## Special thank's to

co worker - **Frédéric Mure**

[Apple2Infinitum](https://app.slack.com/) - digarok

[Brutal Deluxe](https://www.brutaldeluxe.fr/) - Antoine VIGNAU & Olivier ZARDINI

[The Byte Works](https://www.byteworks.us/Products.html) - Mike Westerfield

[GoldenGate](https://goldengate.gitlab.io/) - Kelvin W Sherlock

Have Fun :exclamation:
