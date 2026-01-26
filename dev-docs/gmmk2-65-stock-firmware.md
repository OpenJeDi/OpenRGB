# GMMK2 65 (ANSI/ISO) Stock Firmware Support

## Goal
Add OpenRGB support scaffolding for the Glorious GMMK2 65 using the stock firmware. Focus on key LED layout/mapping first; defer per-key color writes until USB traces are available.

## Constraints / Inputs
- Stock firmware USB protocol is unknown; we only have interface/descriptor data.
- We must follow OpenRGB controller structure and keep code clean.
- Per-key RGB support requires USB traces from Glorious Core on Windows.

## Known Hardware Data
- Device: Glorious GMMK 2 Compact TKL 65%
- ANSI VID:PID: 0x320F:0x5045
- ISO  VID:PID: 0x320F:0x504A
- Interfaces:
  - Vendor HID (usage page 0xFF00, usage 0xFF00) with Report ID 0x04 (8-byte output report in descriptor)
  - Vendor HID (usage page 0xFF01, usage 0x0001) with Report ID 0x07 (255-byte feature report in descriptor)
- Layout data from QMK:
  - ANSI: 67 key LEDs
  - ISO:  68 key LEDs
  - 20 extra LEDs (likely underglow/side)
  - Key positions and LED ordering available in `qmk_firmware/keyboards/gmmk/gmmk2/p65`

## Implementation Plan
### Phase 1: Skeleton + Layout (done)
- Add new controller folder: `Controllers/GloriousGMMK2Controller/`
  - `GloriousGMMK2Controller.h/.cpp`
    - Open HID device(s)
    - Track output interface handle (usage page 0xFF00)
    - Track feature interface handle (usage page 0xFF01)
    - Stub methods for SetLEDs/SetMode that log a warning and return
  - `GloriousGMMK2ControllerDetect.cpp`
    - Detect VID/PID 320F:5045 (ANSI) and 320F:504A (ISO)
    - Prefer the feature interface for the main handle
    - Enumerate for the output interface to keep both handles if available
- Add `RGBController_GloriousGMMK2.h/.cpp`
  - "Keyboard" zone of type `ZONE_TYPE_MATRIX`
  - ANSI/ISO key LED mappings derived from QMK
  - LED names via `RGBControllerKeyNames.h`
  - One "Direct" mode (per-LED color), but no actual write yet
  - "Underglow" zone (20 LEDs) derived from QMK positions; still stubbed

### Phase 2: Trace-Driven Protocol
- Once USB traces are available:
  - Implement per-key frame construction
  - Implement full update and single-LED update
  - Validate against GMMK2 traces

## Capture Checklist (Windows, USBPcap + Wireshark)
1) Report Descriptor capture:
   - Start capture on the device host
   - Unplug/replug the keyboard
   - Stop and save as `gmmk2_report_descriptor.pcapng`
2) Whole-device static colors (separate capture per color):
   - All keys static RED
   - All keys static GREEN
   - All keys static BLUE
   - All keys OFF
3) Single-key changes (separate captures):
   - Only ESC = Red
   - Only A = Green
   - Only 1 = Blue
4) Modes capture:
   - One capture covering all supported modes using a single color (white)
5) Brightness capture:
   - One capture stepping through all brightness levels (if supported)
6) Speed capture:
   - One capture stepping through all speeds for a mode

Notes:
- Isolate the device on its own USB host if possible.
- Add packet comments in Wireshark at the first and last packet of each change.
- Save each capture as its own .pcapng file named with device + action.

## Layout Derivation
- Source: `qmk_firmware/keyboards/gmmk/gmmk2/p65/ansi/ansi.c`, `iso/iso.c`, and `keyboard.json`
- Matrix grid: 5 rows x 16 cols
- LED ordering: matches QMK LED index order for keys
- Underglow positions: QMK flags=2 entries, ordered right side then left side

## Future Work
- Confirm underglow LED order against captures
- Implement full mode/brightness/speed support based on actual traces
- Add device-specific quirks if the output interface varies by OS
- Replace stub write calls with real HID reports from traces

## Cleanup Plan
- Remove this dev doc before final PR/merge, per project instructions.
