# Transfer request policy

This milestone adds a portable C preflight contract for download jobs. It runs before a network backend or filesystem writer is allowed to start.

## Enforced rules

- HTTPS only; empty hosts, embedded user/password credentials and URL fragments are rejected.
- Query strings remain allowed, but callers must not log credential-bearing query values.
- Filenames are plain ASCII basenames with no path separators, whitespace or traversal.
- Destination must equal the selected allowed root plus the validated filename.
- Roots must be under a supported Vita mount and contain no dot segments or duplicate separators.
- A non-zero byte ceiling is mandatory; the hard cap is 4 GiB minus one byte.
- SHA-256 is validated when required or supplied.

The validator is lexical. The future Vita filesystem backend must still reject symlink escapes, stage into a temporary file and atomically finalize only after checksum verification.

## Host tests

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

No HTTP request or filesystem write is performed by this module.
