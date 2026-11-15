import math
import random

import matplotlib.pyplot as plt

N = 30
K_TRUE = 2.0
B_TRUE = 3.0
X_MIN = 0.0
X_MAX = 10.0
SEED = 2026


def main():
    n = N + 10
    sigma = N / 5
    random.seed(SEED)

    x = [X_MIN + i * (X_MAX - X_MIN) / (n - 1) for i in range(n)]
    y_line = [K_TRUE * xi + B_TRUE for xi in x]
    y = [yi + random.gauss(0, sigma) for yi in y_line]

    sx = sum(x)
    sy = sum(y)
    sxx = sum(xi * xi for xi in x)
    syy = sum(yi * yi for yi in y)
    sxy = sum(xi * yi for xi, yi in zip(x, y))

    den = n * sxx - sx ** 2
    a1 = (n * sxy - sx * sy) / den
    a0 = (sy * sxx - sx * sxy) / den

    mx, my = sx / n, sy / n
    s_x = math.sqrt(sxx / n - mx ** 2)
    s_y = math.sqrt(syy / n - my ** 2)
    r = (sxy / n - mx * my) / (s_x * s_y)
    c = r * s_x / s_y
    d = mx - c * my

    y_reg = [a0 + a1 * xi for xi in x]
    y_inv = [(xi - d) / c for xi in x]
    rss = sum((yi - yr) ** 2 for yi, yr in zip(y, y_reg))

    print(f"N = {N}, n = {n}, sigma = {sigma:.3f}\n")
    print(f"{'i':>4}{'x':>10}{'y_line':>10}{'y_noisy':>10}{'y_model':>10}")
    for i in range(n):
        print(f"{i + 1:>4}{x[i]:>10.4f}{y_line[i]:>10.4f}{y[i]:>10.4f}{y_reg[i]:>10.4f}")

    print(f"\nTrue line:         y = {K_TRUE:.4f} * x + {B_TRUE:.4f}")
    print(f"Regression Y on X: y = {a1:.4f} * x + {a0:.4f}")
    print(f"Regression X on Y: x = {c:.4f} * y + {d:.4f}")
    print(f"Regression X on Y in y(x): y = {1 / c:.4f} * x + {-d / c:.4f}")
    print(f"r = {r:.4f}")
    print(f"R = {rss:.4f}")
    print(f"dk = {abs(a1 - K_TRUE):.4f}, db = {abs(a0 - B_TRUE):.4f}")

    plt.figure(figsize=(7, 4.5))
    plt.plot(x, y_line, "k-", label="Вихідна лінія")
    plt.plot(x, y, "ko", label="Зашумлені дані")
    plt.plot(x, y_reg, "k--", label="Регресія Y на X")
    plt.plot(x, y_inv, "k:", linewidth=2, label="Регресія X на Y")
    plt.ylim(min(y) - 2, max(y) + 2)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True, alpha=0.4)
    plt.legend()
    plt.tight_layout()
    plt.savefig("graph.png", dpi=200)


if __name__ == "__main__":
    main()
