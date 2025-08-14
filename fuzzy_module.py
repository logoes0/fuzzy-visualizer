import numpy as np
import skfuzzy as fuzz
from skfuzzy import control as ctrl

# Define fuzzy variables once
fps_var = ctrl.Antecedent(np.arange(0, 120, 1), 'fps')
temp_var = ctrl.Antecedent(np.arange(0, 101, 1), 'temp')
gpu_load_var = ctrl.Antecedent(np.arange(0, 101, 1), 'gpu_load')
vram_var = ctrl.Antecedent(np.arange(0, 101, 1), 'vram_usage')
motion_var = ctrl.Antecedent(np.arange(0, 101, 1), 'motion_intensity')
quality_var = ctrl.Consequent(np.arange(0, 3, 1), 'quality')

# Membership functions
fps_var['low'] = fuzz.trapmf(fps_var.universe, [0, 0, 40, 50])
fps_var['medium'] = fuzz.trimf(fps_var.universe, [45, 60, 75])
fps_var['high'] = fuzz.trapmf(fps_var.universe, [70, 80, 120, 120])

temp_var['cool'] = fuzz.trapmf(temp_var.universe, [0, 0, 50, 60])
temp_var['warm'] = fuzz.trimf(temp_var.universe, [55, 70, 80])
temp_var['hot'] = fuzz.trapmf(temp_var.universe, [75, 85, 100, 100])

gpu_load_var['low'] = fuzz.trapmf(gpu_load_var.universe, [0, 0, 40, 50])
gpu_load_var['high'] = fuzz.trapmf(gpu_load_var.universe, [45, 60, 100, 100])

vram_var['low'] = fuzz.trapmf(vram_var.universe, [0, 0, 50, 60])
vram_var['high'] = fuzz.trapmf(vram_var.universe, [55, 70, 100, 100])

motion_var['static'] = fuzz.trapmf(motion_var.universe, [0, 0, 20, 30])
motion_var['moderate'] = fuzz.trimf(motion_var.universe, [25, 50, 70])
motion_var['fast'] = fuzz.trapmf(motion_var.universe, [65, 80, 100, 100])

quality_var['low'] = fuzz.trimf(quality_var.universe, [0, 0, 1])
quality_var['medium'] = fuzz.trimf(quality_var.universe, [0, 1, 2])
quality_var['high'] = fuzz.trimf(quality_var.universe, [1, 2, 2])

# Rules
rules = [
    ctrl.Rule(fps_var['high'] & temp_var['cool'] & gpu_load_var['low'] & vram_var['low'], quality_var['high']),
    ctrl.Rule(fps_var['medium'] | temp_var['warm'] | gpu_load_var['high'] | vram_var['high'], quality_var['medium']),
    ctrl.Rule(fps_var['low'] | temp_var['hot'], quality_var['low']),
    ctrl.Rule(motion_var['fast'] & fps_var['medium'], quality_var['medium']),
    ctrl.Rule(motion_var['fast'] & fps_var['low'], quality_var['low'])
]

# Build control system once
system = ctrl.ControlSystem(rules)

def compute_quality(fps, temp, gpu_load, vram_usage, motion_intensity):
    """
    Compute fuzzy quality score: 0 = High, 1 = Medium, 2 = Low
    """
    sim = ctrl.ControlSystemSimulation(system)
    sim.input['fps'] = fps
    sim.input['temp'] = temp
    sim.input['gpu_load'] = gpu_load
    sim.input['vram_usage'] = vram_usage
    sim.input['motion_intensity'] = motion_intensity

    sim.compute()
    return int(round(sim.output['quality']))
