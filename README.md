# libbionetio

This lib takes a large xml file of biological networks and decompose it using category(subsystem) information.
The output can be either xml or sbml format. The c++ library was developed by [Hsiang-Yun Wu][yw].

## Installing

### Prerequisites

- [cmake 3.4+][cmake]
```
$ sudo apt install cmake
```
- [tinyxml][tinyxml]
```
$ sudo apt install libtinyxml-dev
```
- [sbml][sbml]
```
$ sudo apt install libsbml5-dev
```

Clone the libbionetio repository into a directory `MLIBBIONETIO`.

```
$ cd LIBBIONETIO
$ mkdir build
$ cd build
$ cmake ../
$ make
```

## Using libbionetio

### export xml format

```
$ ./bionetio -f xml -i "file_path/input_file_name.xml" -o "file_path/output_path_name"
```
or

### export sbml format

```
$ ./bionetio -f sbml -i "file_path/input_file_name.xml" -o "file_path/output_path_name"
```

[cmake]: https://cmake.org/
[tinyxml]: http://www.grinninglizard.com/tinyxml/
[sbml]: https://sbml.org/

[yw]: http://yun-vis.net/
