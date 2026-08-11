# Unit Tests

`UhalUnitTests.cpp` is a dependency-free host test sample. It exercises the SHT3x command path, a valid Modbus request, and invalid Modbus input through fake adapters.

Run it with `ctest --test-dir build --output-on-failure`. Add a dedicated test source when a component's behavior becomes large enough to need focused cases.

