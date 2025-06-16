import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import firwin, freqz

# === 1. Parâmetros do Filtro ===
num_taps = 15                     # Número de coeficientes do FIR (ímpar para simetria)
cutoff_hz = 5                    # Frequência de corte desejada (Hz)
fs = 1179                          # Taxa de amostragem (Hz)
adc_max = 1023                    # Valor máximo do ADC (10 bits)

# === 2. Frequência Normalizada ===
normalized_cutoff = cutoff_hz / (fs / 2)  # Corte normalizado para freq de Nyquist

# === 3. Gerar Coeficientes em Ponto Flutuante ===
float_coeffs = firwin(num_taps, normalized_cutoff, window='hamming')

# === 4. Escalar para Coeficientes Inteiros (int16) ===
scale_factor = adc_max / np.sum(np.abs(float_coeffs))  # Garante que saída fique ~[0,1023]
int16_coeffs = np.round(float_coeffs * scale_factor).astype(np.int16)

# === 5. Mostrar os Coeficientes ===
print("Coeficientes em ponto flutuante:")
print(float_coeffs)

print("\nCoeficientes inteiros (int16):")
print(int16_coeffs)

# === 6. Função para Aplicar Filtro FIR ===
def apply_fir_filter(input_signal, coefficients):
    num_taps = len(coefficients)
    output = []
    for n in range(len(input_signal)):
        acc = 0
        for k in range(num_taps):
            if n - k >= 0:
                acc += input_signal[n - k] * coefficients[k]
        output.append(acc)
    return output

# === 7. Sinal de Entrada (simulação de valores ADC) ===
input_signal = [512, 800, 1020, 800, 512, 300, 0, 300, 512, 800, 1020, 800, 512, 300, 0]
filtered_output = apply_fir_filter(input_signal, int16_coeffs)

# === 8. Normalizar e Truncar Resultado (para simular faixa do ADC) ===
normalized_output = np.clip(filtered_output, 0, 1023)  # Garante que fique em [0, 1023]

# === 9. Plotar Sinal de Entrada e Saída ===
plt.figure()
plt.plot(input_signal, label="Sinal de Entrada (ADC)", marker='o')
plt.plot(normalized_output, label="Saída Filtrada", marker='x')
plt.title("Filtro FIR Aplicado ao Sinal de Entrada")
plt.xlabel("Índice de Amostra")
plt.ylabel("Valor")
plt.grid(True)
plt.legend()
plt.tight_layout()

# === 10. Calcular e Mostrar Resposta em Frequência ===
w, h = freqz(float_coeffs, worN=8000)  # Resposta com coef. em ponto flutuante
frequencies_hz = w * fs / (2 * np.pi)  # Converter de rad/amostra para Hz
magnitude_db = 20 * np.log10(np.abs(h))  # Magnitude em decibéis

# === 11. Plotar Resposta em Frequência ===
plt.figure()
plt.plot(frequencies_hz, magnitude_db)
plt.title("Resposta em Frequência do Filtro FIR")
plt.xlabel("Frequência (Hz)")
plt.ylabel("Ganho (dB)")
plt.axvline(cutoff_hz, color='red', linestyle='--', label="Frequência de Corte")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
# === 12. Resposta ao Impulso ===
impulse = [1.0] + [0.0] * (num_taps - 1)  # Impulso unitário com zeros após
impulse_response = apply_fir_filter(impulse, float_coeffs)  # Usa float para mais precisão

# === 13. Plotar a Resposta ao Impulso ===
plt.figure()
plt.stem(range(len(impulse_response)), impulse_response, basefmt=" ")
plt.title("Resposta ao Impulso do Filtro FIR")
plt.xlabel("n (amostras)")
plt.ylabel("Amplitude")
plt.grid(True)
plt.tight_layout()
plt.show()
