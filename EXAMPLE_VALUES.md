# Example Values for Each Quality Level

Based on the fuzzy logic system with GMM-derived boundaries from `cpu_metrics.csv`.

## HIGH QUALITY (Low Power Consumption) - Quality Level 2

These values result in low power consumption (~70W) and high quality rendering:

### Set 1:
- CPU Load: 20.0
- Temperature: 40.0
- GPU Load: 1.50
- VRAM Usage: 1.00

### Set 2:
- CPU Load: 30.0
- Temperature: 50.0
- GPU Load: 2.00
- VRAM Usage: 2.00

### Set 3:
- CPU Load: 25.0
- Temperature: 45.0
- GPU Load: 1.80
- VRAM Usage: 1.50

## MEDIUM QUALITY (Medium Power Consumption) - Quality Level 1

These values result in medium power consumption (~92W) and medium quality rendering:

### Set 1:
- CPU Load: 55.0
- Temperature: 63.0
- GPU Load: 3.00
- VRAM Usage: 6.00

### Set 2:
- CPU Load: 50.0
- Temperature: 60.0
- GPU Load: 2.80
- VRAM Usage: 5.00

### Set 3:
- CPU Load: 60.0
- Temperature: 65.0
- GPU Load: 3.20
- VRAM Usage: 7.00

## LOW QUALITY (High Power Consumption) - Quality Level 0

These values result in high power consumption (~645W) and low quality rendering:

### Set 1:
- CPU Load: 90.0
- Temperature: 90.0
- GPU Load: 4.00
- VRAM Usage: 30.00

### Set 2:
- CPU Load: 100.0
- Temperature: 95.0
- GPU Load: 4.20
- VRAM Usage: 35.00

### Set 3:
- CPU Load: 120.0
- Temperature: 100.0
- GPU Load: 4.50
- VRAM Usage: 40.00

## Notes

- The GUI sliders have been updated to match the actual CSV data ranges:
  - CPU Load: 0.0 to 200.0
  - Temperature: 25.0 to 120.0
  - GPU Load: 1.0 to 5.0
  - VRAM Usage: 0.0 to 50.0

- Power consumption is calculated using fuzzy membership functions derived from GMM analysis of the dataset.

- Quality is determined by power consumption thresholds:
  - Power ≤ 81W → High Quality (2)
  - 81W < Power ≤ 368W → Medium Quality (1)
  - Power > 368W → Low Quality (0)

