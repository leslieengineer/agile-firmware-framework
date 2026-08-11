# ESP-IDF v6 Compatibility Manifest

## Supported API baseline

The catalog is prepared for the ESP-IDF v6 API generation. A product must pin an exact v6 release, for example in its manifest, lockfile, or CI environment, before an implementation is added.

## Upgrade procedure

1. Read the ESP-IDF migration guide for the target release.
2. Build the affected platform adapter against the new pinned release.
3. Put API shims in `compatibility/` only if required.
4. Run the UHAL interface contract tests and product integration tests.
5. Create `esp_idf_v7.md` only when adopting v7; stable peripheral filenames remain unchanged.
