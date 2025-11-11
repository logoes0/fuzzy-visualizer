"""
fuzzy_module.py

Fuzzy Inference System for graphical quality calculation.
Uses scikit-fuzzy (skfuzzy) for Mamdani-type fuzzy logic with data-driven
membership functions using GMM (Gaussian Mixture Model) approach.

This module:
1. Loads metrics from CSV and generates GMM boundaries for all metrics
2. Uses fuzzy logic to calculate power consumption from 4 input metrics
3. Uses fuzzy logic to determine quality from power consumption
"""

import numpy as np
import warnings
import os

try:
    import skfuzzy as fuzz
    from skfuzzy import control as ctrl
except ImportError:
    print("Error: 'scikit-fuzzy' library not found. Please install it: pip install scikit-fuzzy")
    exit()

try:
    from sklearn.mixture import GaussianMixture
    from sklearn.exceptions import ConvergenceWarning
except ImportError:
    print("Error: 'scikit-learn' library not found. Please install it: pip install scikit-learn")
    exit()

try:
    import pandas as pd
except ImportError:
    print("Error: 'pandas' library not found. Please install it: pip install pandas")
    exit()

# --------------------------------------------------------------------------
# DATA-DRIVEN MEMBERSHIP FUNCTION GENERATION FROM CSV
# --------------------------------------------------------------------------

def load_metrics_data(csv_path=None):
    """
    Load all metrics from CSV file.
    
    Args:
        csv_path: Path to CSV file. If None, tries 'processed_cpu_metrics.csv' 
                 then 'cpu_metrics.csv' in current directory.
    
    Returns:
        Dictionary with arrays for each metric
    """
    if csv_path is None:
        # Try processed_cpu_metrics.csv first, then cpu_metrics.csv
        if os.path.exists("processed_cpu_metrics.csv"):
            csv_path = "processed_cpu_metrics.csv"
        elif os.path.exists("cpu_metrics.csv"):
            csv_path = "cpu_metrics.csv"
        else:
            raise FileNotFoundError(
                "CSV file not found. Expected 'processed_cpu_metrics.csv' or 'cpu_metrics.csv' "
                "in current directory."
            )
    
    try:
        df = pd.read_csv(csv_path)
    except Exception as e:
        raise FileNotFoundError(f"Failed to load CSV file '{csv_path}': {e}")
    
    required_columns = ['cpu load', 'temperature', 'gpu load', 'vram_usage', 'Power Consumption (W)']
    missing_columns = [col for col in required_columns if col not in df.columns]
    if missing_columns:
        raise ValueError(f"CSV file missing required columns: {missing_columns}")
    
    # Convert to numeric, coercing errors to NaN, then drop NaN rows
    for col in required_columns:
        df[col] = pd.to_numeric(df[col], errors='coerce')
    
    # Drop rows with any NaN values
    df = df.dropna()
    
    if len(df) == 0:
        raise ValueError("CSV file contains no valid numeric data")
    
    return {
        'cpu_load': df['cpu load'].values.astype(float),
        'temperature': df['temperature'].values.astype(float),
        'gpu_load': df['gpu load'].values.astype(float),
        'vram_usage': df['vram_usage'].values.astype(float),
        'power_consumption': df['Power Consumption (W)'].values.astype(float)
    }


def generate_gmm_membership_params(data, n_components=3, random_state=42):
    """
    Generates Gaussian membership function parameters using GMM clustering.
    Returns sorted (mean, sigma) tuples for low, medium, high.
    
    Args:
        data: 1D numpy array of data values
        n_components: Number of GMM components (default: 3 for low/medium/high)
        random_state: Random seed for reproducibility
        
    Returns:
        List of (mean, sigma) tuples sorted by mean: [(low_mean, low_sigma), 
        (med_mean, med_sigma), (high_mean, high_sigma)]
    """
    data = np.array(data).flatten()
    
    # Handle edge case: insufficient unique values
    unique_values = np.unique(data)
    if len(unique_values) < n_components:
        # Fallback to percentile-based approach
        low_mean = np.percentile(data, 10)
        med_mean = np.percentile(data, 50)
        high_mean = np.percentile(data, 90)
        data_std = np.std(data)
        low_sigma = data_std * 0.3
        med_sigma = data_std * 0.3
        high_sigma = data_std * 0.3
        return [(low_mean, low_sigma), (med_mean, med_sigma), (high_mean, high_sigma)]
    
    # Reshape for GMM (needs 2D array)
    data_2d = data.reshape(-1, 1)
    
    try:
        model = GaussianMixture(
            n_components=n_components,
            covariance_type="full",
            random_state=random_state
        )
        
        with warnings.catch_warnings():
            warnings.filterwarnings("ignore", category=ConvergenceWarning)
            model.fit(data_2d)
        
        # Extract means and standard deviations
        means = model.means_.flatten()
        
        # Extract covariances and compute standard deviations
        # Handle different covariance shapes from different scikit-learn versions
        covariances = model.covariances_
        if covariances.ndim == 3:
            # Shape: (n_components, 1, 1) - extract diagonal
            sigmas = np.sqrt(covariances[:, 0, 0])
        elif covariances.ndim == 2:
            # Shape: (n_components, 1) or (n_components,)
            if covariances.shape[1] == 1:
                sigmas = np.sqrt(covariances[:, 0])
            else:
                sigmas = np.sqrt(np.diag(covariances))
        else:
            # Shape: (n_components,)
            sigmas = np.sqrt(covariances)
        
        # Combine means and sigmas, then sort by mean
        params = list(zip(means, sigmas))
        params.sort(key=lambda x: x[0])  # Sort by mean ascending
        
        return params
        
    except Exception as e:
        # Fallback to percentile-based approach
        low_mean = np.percentile(data, 10)
        med_mean = np.percentile(data, 50)
        high_mean = np.percentile(data, 90)
        data_std = np.std(data)
        low_sigma = data_std * 0.3
        med_sigma = data_std * 0.3
        high_sigma = data_std * 0.3
        return [(low_mean, low_sigma), (med_mean, med_sigma), (high_mean, high_sigma)]


# Load all metrics from CSV and generate GMM parameters
try:
    metrics_data = load_metrics_data()
    
    # Generate GMM parameters for all metrics
    gmm_params = {}
    metric_ranges = {}
    
    for metric_name, data in metrics_data.items():
        gmm_params[metric_name] = generate_gmm_membership_params(data, n_components=3, random_state=42)
        metric_ranges[metric_name] = (float(data.min()), float(data.max()))
    
    print(f"[fuzzy_module] Loaded {len(metrics_data['cpu_load'])} samples from CSV")
    print(f"[fuzzy_module] GMM boundaries generated for all metrics")
    
except Exception as e:
    print(f"[fuzzy_module] WARNING: Failed to load CSV data: {e}")
    print("[fuzzy_module] Falling back to default synthetic boundaries")
    # Fallback to default values if CSV loading fails
    gmm_params = {
        'cpu_load': [(20.0, 10.0), (50.0, 15.0), (80.0, 10.0)],
        'temperature': [(30.0, 10.0), (60.0, 15.0), (90.0, 10.0)],
        'gpu_load': [(10.0, 5.0), (50.0, 20.0), (90.0, 5.0)],
        'vram_usage': [(10.0, 5.0), (50.0, 20.0), (90.0, 5.0)],
        'power_consumption': [(50.0, 15.0), (100.0, 20.0), (200.0, 50.0)]
    }
    metric_ranges = {
        'cpu_load': (0.0, 100.0),
        'temperature': (20.0, 100.0),
        'gpu_load': (0.0, 100.0),
        'vram_usage': (0.0, 100.0),
        'power_consumption': (0.0, 300.0)
    }

# --------------------------------------------------------------------------
# FUZZY SYSTEM 1: Calculate Power Consumption from 4 Input Metrics
# --------------------------------------------------------------------------

# Input variables for power consumption calculation
cpu_load_var = ctrl.Antecedent(np.linspace(metric_ranges['cpu_load'][0], metric_ranges['cpu_load'][1], 100), 'cpu_load')
temp_var = ctrl.Antecedent(np.linspace(metric_ranges['temperature'][0], metric_ranges['temperature'][1], 100), 'temperature')
gpu_load_var = ctrl.Antecedent(np.linspace(metric_ranges['gpu_load'][0], metric_ranges['gpu_load'][1], 100), 'gpu_load')
vram_var = ctrl.Antecedent(np.linspace(metric_ranges['vram_usage'][0], metric_ranges['vram_usage'][1], 100), 'vram_usage')

# Output variable: Power Consumption
power_var = ctrl.Consequent(np.linspace(metric_ranges['power_consumption'][0], metric_ranges['power_consumption'][1], 100), 
                           'power_consumption')

# Create membership functions for inputs using GMM parameters
# Use narrower sigmas (multiply by 0.5) to reduce overlap and make boundaries clearer
cpu_load_var['low'] = fuzz.gaussmf(cpu_load_var.universe, gmm_params['cpu_load'][0][0], gmm_params['cpu_load'][0][1] * 0.5)
cpu_load_var['medium'] = fuzz.gaussmf(cpu_load_var.universe, gmm_params['cpu_load'][1][0], gmm_params['cpu_load'][1][1] * 0.5)
cpu_load_var['high'] = fuzz.gaussmf(cpu_load_var.universe, gmm_params['cpu_load'][2][0], gmm_params['cpu_load'][2][1] * 0.5)

temp_var['low'] = fuzz.gaussmf(temp_var.universe, gmm_params['temperature'][0][0], gmm_params['temperature'][0][1] * 0.5)
temp_var['medium'] = fuzz.gaussmf(temp_var.universe, gmm_params['temperature'][1][0], gmm_params['temperature'][1][1] * 0.5)
temp_var['high'] = fuzz.gaussmf(temp_var.universe, gmm_params['temperature'][2][0], gmm_params['temperature'][2][1] * 0.5)

gpu_load_var['low'] = fuzz.gaussmf(gpu_load_var.universe, gmm_params['gpu_load'][0][0], gmm_params['gpu_load'][0][1] * 0.5)
gpu_load_var['medium'] = fuzz.gaussmf(gpu_load_var.universe, gmm_params['gpu_load'][1][0], gmm_params['gpu_load'][1][1] * 0.5)
gpu_load_var['high'] = fuzz.gaussmf(gpu_load_var.universe, gmm_params['gpu_load'][2][0], gmm_params['gpu_load'][2][1] * 0.5)

vram_var['low'] = fuzz.gaussmf(vram_var.universe, gmm_params['vram_usage'][0][0], gmm_params['vram_usage'][0][1] * 0.5)
vram_var['medium'] = fuzz.gaussmf(vram_var.universe, gmm_params['vram_usage'][1][0], gmm_params['vram_usage'][1][1] * 0.5)
vram_var['high'] = fuzz.gaussmf(vram_var.universe, gmm_params['vram_usage'][2][0], gmm_params['vram_usage'][2][1] * 0.5)

# Power consumption output membership functions (from GMM)
power_var['low'] = fuzz.gaussmf(power_var.universe, gmm_params['power_consumption'][0][0], gmm_params['power_consumption'][0][1])
power_var['medium'] = fuzz.gaussmf(power_var.universe, gmm_params['power_consumption'][1][0], gmm_params['power_consumption'][1][1])
power_var['high'] = fuzz.gaussmf(power_var.universe, gmm_params['power_consumption'][2][0], gmm_params['power_consumption'][2][1])

# Rules for calculating power consumption from input metrics
# Order matters: more specific rules first, general rules last
power_rules = [
    # High power: all components clearly high (most specific, highest priority)
    ctrl.Rule(cpu_load_var['high'] & temp_var['high'] & gpu_load_var['high'] & vram_var['high'], 
             power_var['high']),
    
    # High power: CPU and temp both high (strong indicator of high power)
    ctrl.Rule(cpu_load_var['high'] & temp_var['high'], power_var['high']),
    
    # High power: CPU high with high GPU or VRAM
    ctrl.Rule(cpu_load_var['high'] & (gpu_load_var['high'] | vram_var['high']), 
             power_var['high']),
    
    # Low power: all components clearly low (strongest low power rule)
    ctrl.Rule(cpu_load_var['low'] & temp_var['low'] & gpu_load_var['low'] & vram_var['low'], 
             power_var['low']),
    
    # Low power: three out of four low
    ctrl.Rule((cpu_load_var['low'] & temp_var['low'] & gpu_load_var['low']) |
             (cpu_load_var['low'] & temp_var['low'] & vram_var['low']) |
             (cpu_load_var['low'] & gpu_load_var['low'] & vram_var['low']) |
             (temp_var['low'] & gpu_load_var['low'] & vram_var['low']),
             power_var['low']),
    
    # Medium power: balanced medium conditions (all medium)
    ctrl.Rule(cpu_load_var['medium'] & temp_var['medium'] & gpu_load_var['medium'] & vram_var['medium'],
             power_var['medium']),
    
    # Medium power: two medium, two low
    ctrl.Rule((cpu_load_var['medium'] & temp_var['medium'] & gpu_load_var['low'] & vram_var['low']) |
             (cpu_load_var['medium'] & gpu_load_var['medium'] & temp_var['low'] & vram_var['low']) |
             (temp_var['medium'] & vram_var['medium'] & cpu_load_var['low'] & gpu_load_var['low']),
             power_var['medium']),
    
    # Medium power: one high but others low/medium (mitigated high)
    ctrl.Rule((cpu_load_var['high'] & temp_var['low'] & gpu_load_var['low'] & vram_var['low']) |
             (temp_var['high'] & cpu_load_var['low'] & gpu_load_var['low'] & vram_var['low']),
             power_var['medium']),
]

power_system = ctrl.ControlSystem(power_rules)
_power_sim = ctrl.ControlSystemSimulation(power_system)

# --------------------------------------------------------------------------
# FUZZY SYSTEM 2: Calculate Quality from Power Consumption
# --------------------------------------------------------------------------

# Input variable: Power Consumption (separate from power_var to avoid circular dependency)
power_input_var = ctrl.Antecedent(np.linspace(metric_ranges['power_consumption'][0], 
                                              metric_ranges['power_consumption'][1], 100), 
                                 'power_input')

# Output variable: Quality
quality_var = ctrl.Consequent(np.linspace(0, 2, 101), 'quality')

# Power input membership functions (use narrower sigmas for clearer boundaries)
power_input_var['low'] = fuzz.gaussmf(power_input_var.universe, 
                                      gmm_params['power_consumption'][0][0], 
                                      gmm_params['power_consumption'][0][1] * 0.6)
power_input_var['medium'] = fuzz.gaussmf(power_input_var.universe, 
                                         gmm_params['power_consumption'][1][0], 
                                         gmm_params['power_consumption'][1][1] * 0.6)
power_input_var['high'] = fuzz.gaussmf(power_input_var.universe, 
                                      gmm_params['power_consumption'][2][0], 
                                      gmm_params['power_consumption'][2][1] * 0.6)

# Quality output membership functions (triangular, fixed)
quality_var['low'] = fuzz.trimf(quality_var.universe, [0, 0, 1])
quality_var['medium'] = fuzz.trimf(quality_var.universe, [0, 1, 2])
quality_var['high'] = fuzz.trimf(quality_var.universe, [1, 2, 2])

# Rules for quality based on power consumption
quality_rules = [
    # High Quality: low power consumption indicates efficient operation
    ctrl.Rule(power_input_var['low'], quality_var['high']),
    
    # Low Quality: high power consumption indicates system stress
    ctrl.Rule(power_input_var['high'], quality_var['low']),
    
    # Medium Quality: moderate power consumption
    ctrl.Rule(power_input_var['medium'], quality_var['medium']),
]

quality_system = ctrl.ControlSystem(quality_rules)
_quality_sim = ctrl.ControlSystemSimulation(quality_system)

# --------------------------------------------------------------------------
# MAIN COMPUTATION FUNCTION (Called from C++)
# --------------------------------------------------------------------------

def compute_quality(cpu_load, temp, gpu_load, vram_usage, motion_intensity):
    """
    Compute fuzzy quality score based on 4 input metrics.
    
    Flow:
    1. Calculate power consumption from (cpu_load, temp, gpu_load, vram_usage) using fuzzy logic
    2. Calculate quality from power consumption using fuzzy logic
    
    Args:
        cpu_load (float): CPU load percentage
        temp (float): Temperature in degrees
        gpu_load (float): GPU load percentage
        vram_usage (float): VRAM usage percentage
        motion_intensity (float): Motion intensity (ignored, kept for compatibility)
    
    Returns:
        int: Quality level (0 = Low, 1 = Medium, 2 = High)
    """
    global _power_sim, _quality_sim, metric_ranges, gmm_params
    
    try:
        # Step 1: Calculate power consumption using fuzzy membership and weighted combination
        # First, determine fuzzy membership for each input
        cpu_low_mem = fuzz.gaussmf(cpu_load, gmm_params['cpu_load'][0][0], gmm_params['cpu_load'][0][1] * 0.5)
        cpu_med_mem = fuzz.gaussmf(cpu_load, gmm_params['cpu_load'][1][0], gmm_params['cpu_load'][1][1] * 0.5)
        cpu_high_mem = fuzz.gaussmf(cpu_load, gmm_params['cpu_load'][2][0], gmm_params['cpu_load'][2][1] * 0.5)
        
        temp_low_mem = fuzz.gaussmf(temp, gmm_params['temperature'][0][0], gmm_params['temperature'][0][1] * 0.5)
        temp_med_mem = fuzz.gaussmf(temp, gmm_params['temperature'][1][0], gmm_params['temperature'][1][1] * 0.5)
        temp_high_mem = fuzz.gaussmf(temp, gmm_params['temperature'][2][0], gmm_params['temperature'][2][1] * 0.5)
        
        gpu_low_mem = fuzz.gaussmf(gpu_load, gmm_params['gpu_load'][0][0], gmm_params['gpu_load'][0][1] * 0.5)
        gpu_med_mem = fuzz.gaussmf(gpu_load, gmm_params['gpu_load'][1][0], gmm_params['gpu_load'][1][1] * 0.5)
        gpu_high_mem = fuzz.gaussmf(gpu_load, gmm_params['gpu_load'][2][0], gmm_params['gpu_load'][2][1] * 0.5)
        
        vram_low_mem = fuzz.gaussmf(vram_usage, gmm_params['vram_usage'][0][0], gmm_params['vram_usage'][0][1] * 0.5)
        vram_med_mem = fuzz.gaussmf(vram_usage, gmm_params['vram_usage'][1][0], gmm_params['vram_usage'][1][1] * 0.5)
        vram_high_mem = fuzz.gaussmf(vram_usage, gmm_params['vram_usage'][2][0], gmm_params['vram_usage'][2][1] * 0.5)
        
        # Calculate overall "low/medium/high" score for the system
        # Use minimum (AND) for low (all must be low), maximum (OR) for high (any high)
        overall_low = min(cpu_low_mem, temp_low_mem, gpu_low_mem, vram_low_mem)
        overall_high = max(cpu_high_mem, temp_high_mem, gpu_high_mem, vram_high_mem)
        overall_med = (cpu_med_mem + temp_med_mem + gpu_med_mem + vram_med_mem) / 4.0
        
        # Map to power consumption using GMM means
        # Use dominant membership approach to avoid skewing from extreme values
        power_low = gmm_params['power_consumption'][0][0]  # ~70W
        power_med = gmm_params['power_consumption'][1][0]  # ~92W
        power_high = gmm_params['power_consumption'][2][0]  # ~645W
        
        # Find dominant membership (highest value)
        max_mem = max(overall_low, overall_med, overall_high)
        
        # Check if inputs are clearly below/above GMM means (even if membership is low)
        cpu_below_low_mean = cpu_load < gmm_params['cpu_load'][0][0]
        temp_below_low_mean = temp < gmm_params['temperature'][0][0]
        gpu_below_low_mean = gpu_load < gmm_params['gpu_load'][0][0]
        vram_below_low_mean = vram_usage < gmm_params['vram_usage'][0][0]
        all_below_low = cpu_below_low_mean and temp_below_low_mean and gpu_below_low_mean and vram_below_low_mean
        
        cpu_above_high_mean = cpu_load > gmm_params['cpu_load'][2][0]
        temp_above_high_mean = temp > gmm_params['temperature'][2][0]
        gpu_above_high_mean = gpu_load > gmm_params['gpu_load'][2][0]
        vram_above_high_mean = vram_usage > gmm_params['vram_usage'][2][0]
        most_above_high = (cpu_above_high_mean and temp_above_high_mean) or (cpu_above_high_mean and gpu_above_high_mean)
        
        if all_below_low or (overall_low > 0.6):
            # Clearly low - use low power
            calculated_power = power_low
        elif most_above_high or (overall_high > 0.8):
            # Clearly high - use high power
            calculated_power = power_high
        elif overall_med > 0.4 and overall_high < 0.6:
            # Clearly medium (medium membership is strong and high is not dominant)
            calculated_power = power_med
        else:
            # Mixed case - use weighted interpolation
            if overall_low > overall_high and overall_low > 0.3:
                # Leaning low
                calculated_power = power_low + (power_med - power_low) * (1 - overall_low)
            elif overall_high > overall_low and overall_high > 0.5:
                # Leaning high
                calculated_power = power_med + (power_high - power_med) * overall_high
            else:
                # Balanced or unclear - use medium
                calculated_power = power_med
        
        # Step 2: Calculate quality from power consumption
        # Use threshold-based approach for clearer boundaries
        power_low_threshold = gmm_params['power_consumption'][0][0] + (gmm_params['power_consumption'][1][0] - gmm_params['power_consumption'][0][0]) / 2  # ~81W
        power_high_threshold = gmm_params['power_consumption'][1][0] + (gmm_params['power_consumption'][2][0] - gmm_params['power_consumption'][1][0]) / 2  # ~368W
        
        if calculated_power <= power_low_threshold:
            # Low power -> High quality
            result = 2
        elif calculated_power <= power_high_threshold:
            # Medium power -> Medium quality
            result = 1
        else:
            # High power -> Low quality
            result = 0
        
        return result
        
    except Exception as e:
        print(f"[fuzzy_module] Error in compute_quality: {e}")
        import traceback
        traceback.print_exc()
        return 1  # Default to medium quality on error
