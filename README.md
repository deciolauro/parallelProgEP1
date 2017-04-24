# Parallel Programming - EP1

This is the first programming exercise for the discipline Introductions to
Parallel and Distributed Programming (MAC5742/2017-1) and consists on a basic
performance analysis about parallelism using the mandelbrot set.

## Parallel Programming using mandelbrot set

The core idea behind this project is to acquire some knowledge about the
difference in performance when using three computational methods over 5
specific regions in the mandelbrot set.

In order to make the analysis, we use a sequential program, a parallel
program using POSIX Threads (pthreads) and a parallel program using open
multi-processing (OpenMP) application programming interface. (API)

Also, to standardize the experiment, either our execution as our tests will
be made by using an Google Compute Engine machine.

Our intention with this work is to infer about the nuances when using those
methods and parallel programming techniques and compare it with the sequencial
program results.

## Getting Started

### Prerequisities

To the present, the only know prerequisites for running/compiling this project
are:

```
A C compiler (gcc is being used in the given Makefile)
A profiler tool (we are using perf, a Linux 2.6+ profiler)
Python 2.7 (with matplotlib, numpy, ...)
```

### (Pre)Installing

We recommend that you fork this repository by logging into your github account
and clicking on the fork icon on the top right side of the screen, because that
will make it easy to contribute with the project via pull requests.

After forking you can get your own copy of project by doing:

```
git clone https://github.com/YOUR-USERNAME/parallelProgEP1.git
```

Alternately, you can just get a copy of this repository by cloning it from
upstream by doing:

```
git clone https://github.com/deciolauro/parallelProgEP1.git
```

However, please note that if using this second method you will not be able to
contribute unless you get a contributor permission from someone inside the
project.


### Compiling

After cloning, you can compile  by doing:

```
cd NAME
./make
```

## Running the tests

You can also, after compiling, run the tests and see the log results by
executing:
```
cd NAME
./run_measurements
```

Because the logs will be stored in the results folder, you may also need
permission to create a new folder if needed.

## Contributing

Please read [CONTRIBUTING](CONTRIBUTING.md) for details on our code of conduct,
and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available,
see the [tags on this repository](https://github.com/deciolauro/parallelProgEP1/tags)

In summary, this means that our Versioning system follow the specific structure:

A version X.Y.Z-K represents:

* X -> MAJOR version change (reserved for incompatible API changes or Major improvements)
* Y -> MINOR version change (reserved for adding functionality and Minor changes)
* Z -> PATCH version change (reserved for bug fixes, typos...)
* K -> PRE-RELEASE version id (an small ASCII alphanumerics and hyphen
[0-9A-Za-z-] with the identifier for the pre-release)

The current build released is 1.0.0

## Authors

* **Candy Tenorio Gonzales (cvtenoriog@gmail.com)**
* **Décio Lauro Soares (deciolauro@gmail.com)**
* **Fatemeh Mosaiyebzadeh (mahshid.msy179@gmail.com)**

See also the list of [contributors](https://github.com/deciolauro/parallelProgEP1/graphs/contributors) who participated in this project.

Please consult our [CONTRIBUTING](CONTRIBUTING.md) for instructions. 

## License

Except for some implementation code produced by [Pedro Bruel](https://github.com/phrb/MAC5742-0219-EP1) which lacks his distribute LICENSE, the rest of this
project is licensed under GPL-3.0 - see the [LICENSE.md](LICENSE.md) file for
details. 

## Copyright

Copyright (C) <2017>  Candy Tenorio Gonzales; Décio Lauro Soares; Fatemeh Mosaiyebzadeh

## Final explanations

A significative part of the code and great part of the insigths and initial
explanations was provided by our TA Pedro Bruel and can be found in his
repository [Pedro Bruel](https://github.com/phrb/MAC5742-0219-EP1).

Since his repository, by the time we are writting this, lacks by which LICENSE
his work is distributed, we took the liberty to adjust his code to our standards.

However, you may notice that we left out the Copyright details on this files and
you may have to ask the author before using it.

## Acknowledgments

We would like to thank:

* Prof. Alfredo Goldman Vel Lejbman, Ph.D for all the support
* Pedro Bruel for the insights, initial orientation and code examples
