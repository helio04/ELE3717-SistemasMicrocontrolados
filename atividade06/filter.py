import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import firwin

# Filter design parameters
num_taps = 15                         # Number of filter coefficients (taps)
cutoff_hz = 2000                      # Desired cutoff frequency in Hz
fs = 9600                             # Sampling rate in Hz
normalized_cutoff = cutoff_hz / (fs / 2)  # Normalize the cutoff

# Generate floating-point FIR filter coefficients
float_coeffs = firwin(num_taps, normalized_cutoff, window='hamming')

# Scale and convert to 16-bit signed integers
scale_factor = 32767 / np.max(np.abs(float_coeffs))  # Scale to make best use of int16 range
int16_coeffs = np.round(float_coeffs * scale_factor).astype(np.int16)

# Display both floating and integer coefficients
print("Floating-point FIR Coefficients:")
print(float_coeffs)

print("\nScaled 16-bit Integer Coefficients:")
print(int16_coeffs)

# Optional: Plot both
plt.stem(int16_coeffs, linefmt='C1-', markerfmt='C1o', basefmt='C1-', use_line_collection=True)
plt.title("Quantized FIR Filter Coefficients (16-bit Integers)")
plt.xlabel("Tap Index")
plt.ylabel("Coefficient Value")
plt.grid(True)
plt.show()
