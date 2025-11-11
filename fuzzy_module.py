"""
fuzzy_module.py

Fuzzy Inference System for graphical quality calculation.
Uses scikit-fuzzy (skfuzzy) for Mamdani-type fuzzy logic with data-driven
membership functions using GMM (Gaussian Mixture Model) approach.

This module is called by the C++ visualizer to compute quality levels
based on system metrics (FPS, temperature, GPU load, VRAM, motion).
"""

import numpy as np
import warnings
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

# --------------------------------------------------------------------------
# DATA-DRIVEN MEMBERSHIP FUNCTION GENERATION
# --------------------------------------------------------------------------

def generate_synthetic_training_data(n_samples=1000):
    """
    Generates synthetic, correlated dataset for graphics metrics.
    Used to create data-driven membership functions via GMM.
    """
    np.random.seed(42)
    
    # Define covariance matrix for correlated metrics
    # [gpu_load, temp, vram_usage, fps]
    # - gpu_load, temp, vram are positively correlated
    # - fps is negatively correlated with all of them
    cov = [
        [1.0, 0.8, 0.7, -0.6],   # gpu_load
        [0.8, 1.0, 0.6, -0.7],   # temp
        [0.7, 0.6, 1.0, -0.5],   # vram_usage
        [-0.6, -0.7, -0.5, 1.0], # fps
    ]
    means = [50, 60, 55, 65]  # Mean values for realistic distributions
    data = np.random.multivariate_normal(means, np.array(cov) * 400, n_samples)
    
    # motion_intensity is independent
    motion = np.random.uniform(0, 100, n_samples)
    
    # Clip values to realistic ranges
    gpu_load = np.clip(data[:, 0], 0, 100)
    temp = np.clip(data[:, 1], 0, 100)
    vram_usage = np.clip(data[:, 2], 0, 100)
    fps = np.clip(data[:, 3], 0, 120)
    
    return {
        'fps': fps,
        'temp': temp,
        'gpu_load': gpu_load,
        'vram_usage': vram_usage,
        'motion_intensity': motion
    }

def generate_gmm_membership_params(data, n_components=3):
    """
    Generates Gaussian membership function parameters using GMM clustering.
    Returns sorted (mean, sigma) tuples for low, medium, high.
    """
    model = GaussianMixture(
        n_components=n_components, 
        covariance_type="full", 
        random_state=42
    )
    
    with warnings.catch_warnings():
        warnings.filterwarnings("ignore", category=ConvergenceWarning)
        model.fit(data.reshape(-1, 1))
    
    # Extract means and standard deviations, sort by mean
    params = sorted(
        zip(model.means_.flatten(), np.sqrt(model.covariances_).flatten())
    )
    
    return params

# Generate training data
training_data = generate_synthetic_training_data(n_samples=500)

# Generate GMM parameters for each metric
fps_gmm = generate_gmm_membership_params(training_data['fps'])
temp_gmm = generate_gmm_membership_params(training_data['temp'])
gpu_load_gmm = generate_gmm_membership_params(training_data['gpu_load'])
vram_gmm = generate_gmm_membership_params(training_data['vram_usage'])
motion_gmm = generate_gmm_membership_params(training_data['motion_intensity'])

# --------------------------------------------------------------------------
# FUZZY VARIABLE DEFINITIONS
# --------------------------------------------------------------------------

# Input variables (Antecedents)
fps_var = ctrl.Antecedent(np.arange(0, 121, 1), 'fps')
temp_var = ctrl.Antecedent(np.arange(0, 101, 1), 'temp')
gpu_load_var = ctrl.Antecedent(np.arange(0, 101, 1), 'gpu_load')
vram_var = ctrl.Antecedent(np.arange(0, 101, 1), 'vram_usage')
motion_var = ctrl.Antecedent(np.arange(0, 101, 1), 'motion_intensity')

# Output variable (Consequent)
# Quality: 0 = Low, 1 = Medium, 2 = High
quality_var = ctrl.Consequent(np.arange(0, 3, 1), 'quality')

# --------------------------------------------------------------------------
# DATA-DRIVEN MEMBERSHIP FUNCTIONS (GMM-based Gaussian)
# --------------------------------------------------------------------------

# FPS membership functions (Gaussian from GMM)
fps_var['low'] = fuzz.gaussmf(fps_var.universe, fps_gmm[0][0], max(fps_gmm[0][1], 5))
fps_var['medium'] = fuzz.gaussmf(fps_var.universe, fps_gmm[1][0], max(fps_gmm[1][1], 5))
fps_var['high'] = fuzz.gaussmf(fps_var.universe, fps_gmm[2][0], max(fps_gmm[2][1], 5))

# Temperature membership functions (Gaussian from GMM)
temp_var['cool'] = fuzz.gaussmf(temp_var.universe, temp_gmm[0][0], max(temp_gmm[0][1], 5))
temp_var['warm'] = fuzz.gaussmf(temp_var.universe, temp_gmm[1][0], max(temp_gmm[1][1], 5))
temp_var['hot'] = fuzz.gaussmf(temp_var.universe, temp_gmm[2][0], max(temp_gmm[2][1], 5))

# GPU Load membership functions (Gaussian from GMM)
gpu_load_var['low'] = fuzz.gaussmf(gpu_load_var.universe, gpu_load_gmm[0][0], max(gpu_load_gmm[0][1], 5))
gpu_load_var['medium'] = fuzz.gaussmf(gpu_load_var.universe, gpu_load_gmm[1][0], max(gpu_load_gmm[1][1], 5))
gpu_load_var['high'] = fuzz.gaussmf(gpu_load_var.universe, gpu_load_gmm[2][0], max(gpu_load_gmm[2][1], 5))

# VRAM Usage membership functions (Gaussian from GMM)
vram_var['low'] = fuzz.gaussmf(vram_var.universe, vram_gmm[0][0], max(vram_gmm[0][1], 5))
vram_var['medium'] = fuzz.gaussmf(vram_var.universe, vram_gmm[1][0], max(vram_gmm[1][1], 5))
vram_var['high'] = fuzz.gaussmf(vram_var.universe, vram_gmm[2][0], max(vram_gmm[2][1], 5))

# Motion Intensity membership functions (Gaussian from GMM)
motion_var['static'] = fuzz.gaussmf(motion_var.universe, motion_gmm[0][0], max(motion_gmm[0][1], 5))
motion_var['moderate'] = fuzz.gaussmf(motion_var.universe, motion_gmm[1][0], max(motion_gmm[1][1], 5))
motion_var['fast'] = fuzz.gaussmf(motion_var.universe, motion_gmm[2][0], max(motion_gmm[2][1], 5))

# Quality output membership functions
# 0 = Low quality, 1 = Medium quality, 2 = High quality
quality_var['low'] = fuzz.trimf(quality_var.universe, [0, 0, 1])
quality_var['medium'] = fuzz.trimf(quality_var.universe, [0, 1, 2])
quality_var['high'] = fuzz.trimf(quality_var.universe, [1, 2, 2])

# --------------------------------------------------------------------------
# FUZZY RULES (Expert-driven rule base)
# --------------------------------------------------------------------------

rules = [
    # High quality rules - all conditions favorable
    ctrl.Rule(
        fps_var['high'] & temp_var['cool'] & gpu_load_var['low'] & vram_var['low'],
        quality_var['high']
    ),
    
    # High quality - good FPS with moderate resources
    ctrl.Rule(
        fps_var['high'] & temp_var['cool'] & motion_var['static'],
        quality_var['high']
    ),
    
    # Medium quality rules - mixed conditions
    ctrl.Rule(
        fps_var['medium'] & temp_var['warm'] & gpu_load_var['medium'],
        quality_var['medium']
    ),
    
    ctrl.Rule(
        fps_var['high'] & (temp_var['warm'] | gpu_load_var['high']),
        quality_var['medium']
    ),
    
    ctrl.Rule(
        fps_var['medium'] & motion_var['moderate'],
        quality_var['medium']
    ),
    
    # Low quality rules - poor conditions
    ctrl.Rule(
        fps_var['low'] | temp_var['hot'],
        quality_var['low']
    ),
    
    ctrl.Rule(
        temp_var['hot'] & gpu_load_var['high'],
        quality_var['low']
    ),
    
    ctrl.Rule(
        fps_var['low'] & motion_var['fast'],
        quality_var['low']
    ),
    
    ctrl.Rule(
        vram_var['high'] & gpu_load_var['high'] & fps_var['low'],
        quality_var['low']
    ),
    
    # Additional balanced rules
    ctrl.Rule(
        fps_var['medium'] & temp_var['cool'] & gpu_load_var['low'],
        quality_var['medium']
    ),
]

# --------------------------------------------------------------------------
# CONTROL SYSTEM (Built once at module load)
# --------------------------------------------------------------------------

system = ctrl.ControlSystem(rules)
# Create simulation object once and reuse (cached for performance)
_cached_sim = ctrl.ControlSystemSimulation(system)

# --------------------------------------------------------------------------
# MAIN COMPUTATION FUNCTION (Called from C++)
# --------------------------------------------------------------------------

def compute_quality(fps, temp, gpu_load, vram_usage, motion_intensity):
    """
    Compute fuzzy quality score based on system metrics using data-driven
    GMM-based Gaussian membership functions.
    
    Args:
        fps (float): Frames per second (0-120)
        temp (float): GPU temperature (0-100)
        gpu_load (float): GPU load percentage (0-100)
        vram_usage (float): VRAM usage percentage (0-100)
        motion_intensity (float): Motion intensity (0-100)
    
    Returns:
        int: Quality level (0 = Low, 1 = Medium, 2 = High)
    
    This function is called by the C++ application via Python's C API.
    Uses cached simulation object to avoid recreation overhead.
    """
    # Use cached simulation object (resets inputs automatically)
    sim = _cached_sim
    
    # Set input values
    sim.input['fps'] = float(fps)
    sim.input['temp'] = float(temp)
    sim.input['gpu_load'] = float(gpu_load)
    sim.input['vram_usage'] = float(vram_usage)
    sim.input['motion_intensity'] = float(motion_intensity)
    
    # Compute fuzzy inference
    try:
        sim.compute()
        quality = sim.output['quality']
        
        # Round to nearest integer (0, 1, or 2)
        result = int(round(quality))
        
        # Clamp to valid range
        result = max(0, min(2, result))
        
        return result
    except Exception as e:
        return 1  # Default to medium quality on error