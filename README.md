# PSBBV Downloads

**SPDW Factory · Transfers with receipts, not surprises.**

PSBBV Downloads owns download and transfer queues for PSBBV. It provides a shared operation experience while PSBBV Core owns global navigation, focus, lifecycle, permissions and node registration.

## Scope

- Queue, pause, resume, cancel and inspect supported downloads.
- Track progress, byte counts, speed, ETA when meaningful, source and destination.
- Validate URLs, content length, file type and destination before transfer.
- Support safe local transfer workflows and approved remote endpoints.
- Verify integrity using trusted checksums or signatures when supplied.
- Expose job state and progress through the versioned PSBBV Node API.

## Architecture rules

- Follow the Node API and manifest contract in [PSBBV Core](https://github.com/SilverCrow2323/PSBBV).
- All jobs are asynchronous, cancellable and bounded; no blocking network or filesystem work on the UI thread.
- Enforce destination-root containment, reject traversal and symlink escapes, and use temporary files plus atomic finalisation.
- Never overwrite user data without explicit confirmation.
- Validate HTTPS certificates and hostnames; no downgrade or insecure fallback.
- Do not redistribute copyrighted content or persist credentials in download URLs or logs.

## Delivery order

1. Define a portable transfer-job state machine and error taxonomy.
2. Implement fixture-driven URL, filename and destination validation.
3. Add a bounded single-download engine with checksum verification.
4. Add queue persistence and safe resume semantics.
5. Integrate transfer progress with Core's shared operation UI.
6. Test interrupted downloads, corrupted payloads, low storage and network loss on Vita.

## Quality gates

State-machine tests, traversal/symlink tests, atomic-write tests, checksum mismatch tests, bounded-memory tests, VitaSDK build and physical network/storage validation. A desktop transfer test is not Vita hardware certification.

## Status

Repository bootstrap and product boundary only. Planned features are not yet implemented. English UI/docs and SPDW Factory identity are required.
