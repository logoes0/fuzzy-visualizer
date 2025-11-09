#!/usr/bin/env bash
# Test script to verify Python modules can be imported

echo "Testing Python environment..."
echo "Python executable: $(which python)"
echo "Python version: $(python --version)"
echo ""

echo "Testing module imports..."
python -c "
import sys
print('Python paths:')
for p in sys.path[:5]:
    print(f'  {p}')
print('  ...')
print()

try:
    import numpy
    print('✓ numpy:', numpy.__version__)
except Exception as e:
    print('✗ numpy:', e)

try:
    import sklearn
    print('✓ scikit-learn:', sklearn.__version__)
except Exception as e:
    print('✗ scikit-learn:', e)

try:
    import skfuzzy
    print('✓ scikit-fuzzy: available')
except Exception as e:
    print('✗ scikit-fuzzy:', e)

print()
print('Importing fuzzy_module...')
try:
    import fuzzy_module
    print('✓ fuzzy_module imported')
    result = fuzzy_module.compute_quality(60, 50, 30, 40, 20)
    print(f'✓ compute_quality test: {result}')
except Exception as e:
    print(f'✗ fuzzy_module error: {e}')
    import traceback
    traceback.print_exc()
"

