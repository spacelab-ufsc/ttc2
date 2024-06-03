# RSCLib - Reed-Solomon C Library

<a href="https://github.com/mgm8/rsclib/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/mgm8/rsclib?style=for-the-badge">
</a>
<a href="https://github.com/mgm8/rsclib/actions">
    <img src="https://img.shields.io/github/workflow/status/mgm8/rsclib/Test?style=for-the-badge">
</a>

## Overview

Based on FEC-3.0.1.

## Compiling

```
mkdir build
cd build
cmake ..
make
```

### Installing

```
sudo make install
```

## Usage Example

```c
#include <rsc/rsc.h>

int main()
{
    reed_solomon_t rs16 = {0};

    rsc_init(8, 0x187, 112, 11, 16, 208, &rs16);

    uint8_t data[32] = {0};
    uint8_t par[32] = {0};
    uint8_t par_len = 0;

    uint8_t i = 0;
    for(i = 0; i < 32; i++)
    {
        data[i] = i;
    }

    rsc_encode(&rs16, data, par, &par_len);

    return 0;
}
```

## License

This project is licensed under LGPLv3 license.
