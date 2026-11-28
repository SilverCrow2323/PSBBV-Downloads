# Roadmap — PSBBV Downloads

## M0 — Contract foundation
- [x] Define node ownership and its boundary with PSBBV Core.
- [x] Add a versioned node manifest and pinned schema copy.
- [x] Validate the manifest and reject malformed contract fixtures.
- [x] Add identity/readiness regression tests.

## M1 — Portable domain model
- [ ] Define typed records, state transitions and error taxonomy for downloads.
- [ ] Add fixture-driven tests for malformed input, cancellation and partial failure.
- [ ] Add path/URL validation, bounded reads and integrity checks where applicable.

## M2 — Safe Vita adapter
- [ ] Implement the built-in node against the current Core Node API.
- [ ] Add VitaSDK cross-build and VPK inspection gates.
- [ ] Make long-running work cancellable and keep the UI thread responsive.
- [ ] Require explicit confirmation for destructive operations and preserve rollback state.

## M3 — Physical qualification
- [ ] Test low-storage, interrupted operation, suspend/resume and recovery on physical Vita hardware.
- [ ] Verify logs and exports do not contain credentials or device identifiers.
- [ ] Promote manifest availability from planned only after acceptance evidence exists.

## Acceptance
Manifest identity/schema pass; domain capabilities and permissions are least-privilege; host tests cover invalid input and failure paths; VitaSDK builds; physical behavior is recorded separately from CI. No credentials, copyrighted payloads or unsupported privileged operations are shipped.

## Release target
Remain at the current 0.1.0 / planned baseline until the portable model, Vita adapter and device recovery gates are complete.

## Evening integration evidence — 2026-11-28

- Verified [Downloads node validation #46124458763](https://github.com/SilverCrow2323/PSBBV-Downloads/actions/runs/46124458763); transfer completion host tests passed in [run #45844581112](https://github.com/SilverCrow2323/PSBBV-Downloads/actions/runs/45844581112).
- Remaining gate: integrate transfer lifecycle/progress with Core and test interrupted/resumed transfers on Vita storage.
