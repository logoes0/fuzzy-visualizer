"""
quality_fuzzy_system.py

A complete, standalone fuzzy inference system that outputs a discrete quality score
in {0,1,2} (0=Low, 1=Medium, 2=High) based on power consumption.

This script:
1. Loads power consumption data from a CSV file
2. Uses Gaussian Mixture Models (GMM) to derive membership function parameters for power consumption
3. Builds a fuzzy inference system with expert-defined rules based on power consumption
4. Provides a compute_quality() function for real-time quality assessment

Dependencies:
    numpy, pandas, scikit-learn, scikit-fuzzy

Installation:
    pip install numpy pandas scikit-learn scikit-fuzzy

Usage:
    python quality_fuzzy_system.py
"""

import numpy as np
import pandas as pd
from typing import List, Tuple
import warnings

# Suppress convergence warnings from GMM
warnings.filterwarnings('ignore', category=UserWarning)

try:
    from sklearn.mixture import GaussianMixture
except ImportError:
    raise ImportError("scikit-learn is required. Install with: pip install scikit-learn")

try:
    import skfuzzy as fuzz
    from skfuzzy import control as ctrl
except ImportError:
    raise ImportError("scikit-fuzzy is required. Install with: pip install scikit-fuzzy")

# Global cached simulation object
_GLOBAL_SIM = None


def generate_gmm_membership_params(data_1d: np.ndarray, n_components: int = 3, 
                                   random_state: int = 42) -> List[Tuple[float, float]]:
    """
    Generate membership function parameters using Gaussian Mixture Model.
    
    Fits a GMM to 1D data and extracts (mean, sigma) tuples for each component,
    sorted by mean (lowest first).
    
    Args:
        data_1d: 1D numpy array of data values
        n_components: Number of GMM components (default: 3 for low/medium/high)
        random_state: Random seed for reproducibility
        
    Returns:
        List of (mean, sigma) tuples sorted by mean: [(low_mean, low_sigma), 
        (med_mean, med_sigma), (high_mean, high_sigma)]
    """
    data_1d = np.array(data_1d).flatten()
    
    # Handle edge case: insufficient unique values
    unique_values = np.unique(data_1d)
    if len(unique_values) < n_components:
        print(f"Warning: Only {len(unique_values)} unique values found, "
              f"using percentile-based fallback for GMM parameters")
        # Fallback to percentile-based approach
        low_mean = np.percentile(data_1d, 10)
        med_mean = np.percentile(data_1d, 50)
        high_mean = np.percentile(data_1d, 90)
        # Estimate sigma from data spread
        data_std = np.std(data_1d)
        low_sigma = data_std * 0.3
        med_sigma = data_std * 0.3
        high_sigma = data_std * 0.3
        return [(low_mean, low_sigma), (med_mean, med_sigma), (high_mean, high_sigma)]
    
    # Reshape for GMM (needs 2D array)
    data_2d = data_1d.reshape(-1, 1)
    
    try:
        # Fit GMM
        gmm = GaussianMixture(n_components=n_components, random_state=random_state)
        gmm.fit(data_2d)
        
        # Extract means (shape: (n_components, 1))
        means = gmm.means_.flatten()
        
        # Extract covariances and compute standard deviations
        # Handle different covariance shapes from different scikit-learn versions
        covariances = gmm.covariances_
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
        print(f"Warning: GMM fitting failed ({e}), using percentile-based fallback")
        # Fallback to percentile-based approach
        low_mean = np.percentile(data_1d, 10)
        med_mean = np.percentile(data_1d, 50)
        high_mean = np.percentile(data_1d, 90)
        data_std = np.std(data_1d)
        low_sigma = data_std * 0.3
        med_sigma = data_std * 0.3
        high_sigma = data_std * 0.3
        return [(low_mean, low_sigma), (med_mean, med_sigma), (high_mean, high_sigma)]


def load_and_validate_data(csv_path: str = "processed_cpu_metrics.csv") -> pd.DataFrame:
    """
    Load CSV file and validate required column exists.
    
    Args:
        csv_path: Path to CSV file
        
    Returns:
        DataFrame with validated column
        
    Raises:
        FileNotFoundError: If CSV file doesn't exist
        ValueError: If required column is missing
    """
    try:
        df = pd.read_csv(csv_path)
    except FileNotFoundError:
        raise FileNotFoundError(f"CSV file '{csv_path}' not found in current directory")
    
    required_column = 'Power Consumption (W)'
    if required_column not in df.columns:
        raise ValueError(f"Missing required column: '{required_column}'")
    
    return df


def build_fuzzy_system(df: pd.DataFrame):
    """
    Build the complete fuzzy inference system from power consumption data.
    
    Args:
        df: DataFrame with power consumption data
        
    Returns:
        Tuple of (ControlSystem, dict of GMM parameters)
    """
    # Step 1: Generate GMM parameters for power consumption
    power_data = df['Power Consumption (W)'].values
    power_params = generate_gmm_membership_params(power_data, n_components=3, random_state=42)
    
    gmm_params = {
        'power_consumption': {
            'low': power_params[0],    # Lowest mean -> low
            'medium': power_params[1], # Middle mean -> medium
            'high': power_params[2]    # Highest mean -> high
        }
    }
    
    power_min, power_max = power_data.min(), power_data.max()
    
    print("Generating GMM membership parameters for power consumption...")
    print(f"  power_consumption: low={power_params[0]}, medium={power_params[1]}, high={power_params[2]}")
    
    # Step 2: Create fuzzy input variable (Antecedent) with dynamic universe
    print("\nCreating fuzzy input variable...")
    power_consumption = ctrl.Antecedent(np.linspace(power_min, power_max, 100), 
                                       'power_consumption')
    
    # Step 3: Create fuzzy output variable (Consequent) with fixed universe
    quality = ctrl.Consequent(np.linspace(0, 2, 101), 'quality')
    
    # Define output membership functions (triangular, fixed)
    quality['low'] = fuzz.trimf(quality.universe, [0, 0, 1])
    quality['medium'] = fuzz.trimf(quality.universe, [0, 1, 2])
    quality['high'] = fuzz.trimf(quality.universe, [1, 2, 2])
    
    # Step 4: Create input membership functions using GMM parameters
    print("\nCreating input membership functions...")
    params = gmm_params['power_consumption']
    # Map GMM components (sorted by mean: low=0, medium=1, high=2) to labels
    power_consumption['low'] = fuzz.gaussmf(power_consumption.universe, 
                                            params['low'][0], params['low'][1])
    power_consumption['medium'] = fuzz.gaussmf(power_consumption.universe, 
                                                params['medium'][0], params['medium'][1])
    power_consumption['high'] = fuzz.gaussmf(power_consumption.universe, 
                                             params['high'][0], params['high'][1])
    
    # Step 5: Define fuzzy rules based on power consumption
    print("\nDefining fuzzy rules...")
    rules = [
        # High Quality: low power consumption indicates efficient operation
        ctrl.Rule(power_consumption['low'], quality['high']),
        
        # Low Quality: high power consumption indicates system stress
        ctrl.Rule(power_consumption['high'], quality['low']),
        
        # Medium Quality: moderate power consumption
        ctrl.Rule(power_consumption['medium'], quality['medium']),
    ]
    
    # Step 6: Build control system
    print("Building control system...")
    system = ctrl.ControlSystem(rules)
    
    return system, gmm_params


def initialize_global_sim(system):
    """
    Initialize or reset the global cached simulation object.
    
    Args:
        system: ControlSystem object
    """
    global _GLOBAL_SIM
    
    if _GLOBAL_SIM is None:
        _GLOBAL_SIM = ctrl.ControlSystemSimulation(system)
    else:
        # Try to reset if method exists, otherwise recreate
        try:
            _GLOBAL_SIM.reset()
        except AttributeError:
            _GLOBAL_SIM = ctrl.ControlSystemSimulation(system)


def compute_quality(power: float) -> int:
    """
    Compute quality score (0=Low, 1=Medium, 2=High) from power consumption.
    
    Args:
        power: Power consumption in watts
        
    Returns:
        Integer quality score: 0 (Low), 1 (Medium), or 2 (High)
        
    Raises:
        TypeError: If input is not numeric
    """
    global _GLOBAL_SIM
    
    # Validate input is numeric
    if not isinstance(power, (int, float, np.number)):
        raise TypeError(f"power must be numeric, got {type(power).__name__}")
    
    # Ensure global sim is initialized
    if _GLOBAL_SIM is None:
        raise RuntimeError("Fuzzy system not initialized. Call initialize_global_sim() first.")
    
    # Reset simulation before use
    try:
        _GLOBAL_SIM.reset()
    except AttributeError:
        # Recreate if reset not available
        system = _GLOBAL_SIM.ctrl  # Get system from existing sim
        _GLOBAL_SIM = ctrl.ControlSystemSimulation(system)
    
    # Set input
    _GLOBAL_SIM.input['power_consumption'] = float(power)
    
    # Compute
    _GLOBAL_SIM.compute()
    
    # Extract crisp output and convert to integer label
    crisp_output = _GLOBAL_SIM.output['quality']
    quality_int = int(round(crisp_output))
    
    # Clip to valid range [0, 2]
    quality_int = max(0, min(2, quality_int))
    
    return quality_int


if __name__ == "__main__":
    print("=" * 70)
    print("Fuzzy Quality Inference System")
    print("=" * 70)
    
    # Load data
    print("\nLoading data...")
    try:
        df = load_and_validate_data("processed_cpu_metrics.csv")
    except FileNotFoundError:
        # Try alternative filename
        print("Trying alternative filename 'cpu_metrics.csv'...")
        df = load_and_validate_data("cpu_metrics.csv")
    print(f"Loaded {len(df)} data points")
    
    # Build fuzzy system
    system, gmm_params = build_fuzzy_system(df)
    
    # Initialize global simulation
    initialize_global_sim(system)
    print("\nFuzzy system initialized and ready for use.")
    
    # Print GMM parameters summary
    print("\n" + "=" * 70)
    print("GMM-Derived Membership Function Parameters")
    print("=" * 70)
    params = gmm_params['power_consumption']
    print(f"\npower_consumption:")
    print(f"  Low:    mean={params['low'][0]:.2f}W,    sigma={params['low'][1]:.2f}W")
    print(f"  Medium: mean={params['medium'][0]:.2f}W, sigma={params['medium'][1]:.2f}W")
    print(f"  High:   mean={params['high'][0]:.2f}W,   sigma={params['high'][1]:.2f}W")
    
    # Example tests
    print("\n" + "=" * 70)
    print("Example Tests")
    print("=" * 70)
    
    # Get power consumption range
    power_min, power_max = df['Power Consumption (W)'].min(), df['Power Consumption (W)'].max()
    
    # Low power example (lower quartile)
    power_low = np.percentile(df['Power Consumption (W)'], 25)
    quality_low = compute_quality(power_low)
    quality_labels = {0: "Low", 1: "Medium", 2: "High"}
    
    print(f"\n1. Low Power Consumption Example:")
    print(f"   Power Consumption: {power_low:.2f}W")
    print(f"   → Quality: {quality_low} ({quality_labels[quality_low]})")
    
    # Medium power example (median)
    power_medium = np.percentile(df['Power Consumption (W)'], 50)
    quality_medium = compute_quality(power_medium)
    
    print(f"\n2. Medium Power Consumption Example:")
    print(f"   Power Consumption: {power_medium:.2f}W")
    print(f"   → Quality: {quality_medium} ({quality_labels[quality_medium]})")
    
    # High power example (upper quartile)
    power_high = np.percentile(df['Power Consumption (W)'], 75)
    quality_high = compute_quality(power_high)
    
    print(f"\n3. High Power Consumption Example:")
    print(f"   Power Consumption: {power_high:.2f}W")
    print(f"   → Quality: {quality_high} ({quality_labels[quality_high]})")
    
    print("\n" + "=" * 70)
    print("Quality Mapping: 0=Low, 1=Medium, 2=High")
    print("=" * 70)
    print("\nScript complete. The compute_quality() function is ready for use.")

