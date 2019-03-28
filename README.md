# ScooterProtocol
C++ implementation of Ninebot's ES2 Internal Serial Communication Protocol.

# Build instructions
Standard CMake build:
```
$ mkdir build
$ cd build
$ cmake ..
$ make install
```
This lib is so small that you can add it as a git submodule instead.

# Usage

implement the abstract class:

``` C++

#include <NinebotMessageIntercept.hpp>

#include "mySerialDefinition.hpp"

class StreamImplem : NinebotMessageStream
{
    private:
        Serial mySerial; // Some serial implementation.

    public:
        void write(int a)
        {
            mySerial.myWrite(a);
        }
        
        int read()
        {
            return mySerial.myRead();
        }
};

```

Use this new class as input to other methods like:

```C++

StreamImplem myStream();
NinebotMessageIntercept myIntercept(&myStream, device);

```
