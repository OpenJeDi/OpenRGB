# GMMK2 65 (ANSI) Stock Firmware Support

## Goal
Add OpenRGB support scaffolding for the Glorious GMMK2 65 (ANSI, VID:PID 320F:5045) using the stock firmware. Focus on key LED layout/mapping first; defer per-key color writes until USB traces are available.

## Constraints / Inputs
- Stock firmware USB protocol is unknown; we only have interface/descriptor data.
- We must follow OpenRGB controller structure and keep code clean.
- Per-key RGB support requires USB traces from Glorious Core on Windows.

## Known Hardware Data
- Device: Glorious GMMK 2 Compact TKL 65%
- VID:PID: 0x320F:0x5045 (ANSI)
- Interfaces:
  - Vendor HID (usage page 0xFF00, usage 0xFF00) with Report ID 0x04 (8-byte output report in descriptor)
  - Vendor HID (usage page 0xFF01, usage 0x0001) with Report ID 0x07 (255-byte feature report in descriptor)
- Layout data from QMK:
  - 67 key LEDs
  - 20 extra LEDs (likely underglow/side), excluded for now
  - Key positions and LED ordering available in `qmk_firmware/keyboards/gmmk/gmmk2/p65/ansi`

## Implementation Plan
### Phase 1: Skeleton + Layout
- Add new controller folder: `Controllers/GloriousGMMK2Controller/`
  - `GloriousGMMK2Controller.h/.cpp`
    - Open HID device(s)
    - Track output interface handle (usage page 0xFF00)
    - Track feature interface handle (usage page 0xFF01)
    - Provide stub methods for SetLEDs/SetMode that log a warning and return
  - `GloriousGMMK2ControllerDetect.cpp`
    - Detect VID/PID 320F:5045
    - Prefer the feature interface for the main handle
    - Enumerate for the output interface to keep both handles if available
- Add `RGBController_GloriousGMMK2.h/.cpp`
  - Define a single “Keyboard” zone of type `ZONE_TYPE_MATRIX`
  - Use ANSI key LED mapping (67 keys) derived from QMK
  - LED names via `RGBControllerKeyNames.h`
  - One “Direct” mode (per‑LED color), but no actual write yet

### Phase 2: Trace‑Driven Protocol
- Once USB traces are available:
  - Implement per‑key frame construction
  - Implement full update and single‑LED update
  - Validate against GMMK2 traces

## Layout Derivation (ANSI)
- Source: `qmk_firmware/keyboards/gmmk/gmmk2/p65/ansi/ansi.c` and `keyboard.json`
- Matrix grid: 5 rows × 16 cols
- LED ordering: matches QMK LED index order for keys 0–66
- Exclude LED indices 69–88 (extra LEDs)

## Future Work
- Add support for ISO variant (VID:PID 320F:504A) + ISO layout map
- Add underglow/side LEDs as a separate zone if desired
- Implement full mode/brightness/speed support based on actual traces
- Add device-specific quirks if the output interface varies by OS
- Replace stub write calls with real HID reports from traces

## Cleanup Plan
- Remove this dev doc before final PR/merge, per project instructions.
