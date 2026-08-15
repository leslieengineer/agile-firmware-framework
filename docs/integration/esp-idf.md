# ESP-IDF Integration

The framework is a standalone host CMake catalog, not an ESP-IDF component tree. Do not point `EXTRA_COMPONENT_DIRS` at framework `components/`.

The parent firmware provides small ESP-IDF bridge components. Each bridge explicitly lists selected framework sources, public include directories and dependencies. Product profiles decide which bridge directories are passed through `EXTRA_COMPONENT_DIRS` before `project.cmake` is included.

Use explicit source lists. Do not copy framework source into the product and do not use a global facade header.
