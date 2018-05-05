# Birch Examples

Example models for the Birch probabilistic programming language.


## License

Birch is open source software.

It is licensed under the Apache License, Version 2.0 (the "License"); you may not use it except in compliance with the License. You may obtain a copy of the License at <http://www.apache.org/licenses/LICENSE-2.0>.


## Installation

To build and install, use:

    birch build
    birch install

Most of the examples will output to the `output/` directory, which you will need to create:

    mkdir output

See the documentation for the individual model classes in the `bi/model` directory, or use the [online documentation](https://birch-lang.org/documentation/examples/) for suggested commands to run.


## Acknowledgements

This package contains data sets that have been prepared into JSON format for Birch. They are:

  * A [bike sharing data set](https://archive.ics.uci.edu/ml/datasets/bike+sharing+dataset) from the Capital Bikeshare system in Washington D.C., studied in [Fanaee-T and Gama (2014)](/index.md#references).
  
  * A russian influenza data set from [Anonymous (1978)](/index.md#references).


## References

  1. Anonymous (1978). Influenza in a boarding school. *British Medical Journal*. **1**:587.

  2. H. Fanaee-T and J. Gama (2014). [Event labeling combining ensemble detectors and background knowledge](http://dx.doi.org/10.1007/s13748-013-0040-3). *Progress in Artificial Intelligence*. **2**:113-127.

  3. F. Lindsten and T. B. Schön (2010). Identification of mixed linear/nonlinear state-space models. In *49th IEEE Conference on Decision and Control (CDC)*. 6377–6382.

  4. L.M. Murray, D. Lundén, J. Kudlicka, D. Broman and T.B. Schön (2018). [Delayed Sampling and Automatic Rao&ndash;Blackwellization of Probabilistic Programs](https://arxiv.org/abs/1708.07787). In *Proceedings of the 21st International Conference on Artificial Intelligence and Statistics (AISTATS) 2018*, Lanzarote, Spain.


## Version history

### v0.0.0

* Pre-release.
