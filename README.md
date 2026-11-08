# ⚛️ Toryum Core AI — Molten Salt Reactor (MSR) Physics Engine & Cyber-Physical Dashboard

![Status](https://img.shields.io/badge/STATUS-NOMINAL%20%2F%20AI%20ACTIVE-10b981?style=for-the-badge&logo=c%2B%2B)
![Deployment](https://img.shields.io/badge/GITHUB_PAGES-ONLINE-06b6d4?style=for-the-badge&logo=github)
![Tech Stack](https://img.shields.io/badge/STACK-C%2B%2B20_%7C_RK4_%7C_HTML5_%7C_Chart.js-3b82f6?style=for-the-badge)
![License](https://img.shields.io/badge/LICENSE-MIT-purple?style=for-the-badge)

---

## 🌐 Live Interactive System Demo

Experience the numerical simulation engine and interact with the physical reactor controls directly in your browser, without local installation:

👉 **[Launch Live Toryum Core AI Simulation Dashboard](https://kallymeire.github.io/Toryum-Core-AI/)**

---

## 📌 Executive Summary & System Architecture

**Toryum Core AI** is a high-performance cyber-physical simulation platform designed to model the kinetic and thermal-hydraulic behavior of a liquid-fueled **Molten Salt Reactor (MSR)** using the Thorium–Uranium-233 ($^{232}\text{Th} / ^{233}\text{U}$) breeder fuel cycle.

The system couples a native **C++20 numerical integration kernel** with an interactive, Sci-Fi-themed real-time visual telemetry dashboard built with HTML5, dynamic SVG element rendering, and Chart.js. To prevent prompt thermal runaway during operational transients, the core features an autonomous **Proportional-Integral-Derivative (PID) AI Guardrail** that actively regulates excess reactivity through dynamic control rod positioning.

```text
                                  +---------------------------------------+
                                  |      External Reactive Perturbations  |
                                  +-------------------+-------------------+
                                                      |
                                                      v
  +--------------------+    Control Action    +-------+-------+    System State    +--------------------+
  |  AI PID Guardrail  | -------------------> | Physics Engine| -----------------> | Telemetry Dashboard|
  | (Feedback Control) |                      | (C++20 / RK4) |                    | (SVG + Chart.js)   |
  +--------------------+                      +-------+-------+                    +--------------------+
            ^                                         |                                      |
            |                                         v                                      |
            +--------------------------------- Thermal Error --------------------------------+
                                              e(t) = T_ref - T(t)
```

---

## 🚀 Key Features & Capabilities

- **C++20 Numerical Physics Kernel (`main.cpp`):** implements a high-precision 4th-order Runge-Kutta (RK4) integration algorithm to evaluate the stiff non-linear differential equations that govern prompt/delayed kinetics and heat exchange.
- **Autonomous AI Guardrail (PID Controller):** closed-loop automated feedback control system ($K_p = 1.2$, $K_i = 0.05$, $K_d = 0.1$) that monitors thermal deviations and dynamically adjusts control rod insertion to prevent prompt supercritical excursions.
- **Cyber-Physical Sci-Fi Dashboard (`index.html`):** dark-mode responsive interface with a real-time vector visualizer that depicts the primary salt coolant loop flow rate, core luminosity shifts, and control rod displacements.
- **High-Frequency Telemetry Analytics:** dynamic real-time time-series rendering powered by Chart.js, tracking Core Bulk Temperature ($^\circ\text{C}$) and Relative Neutron Flux ($\Phi$).
- **Real-Time Shell Logging Stream:** embedded terminal window that renders live inter-process communication (IPC) log streams directly from the backend execution loop.
- **Perturbation Stress-Testing Suite:** manual command controls that allow instant injection of positive reactivity spikes ($+150$ pcm), negative thermal drops ($-100$ pcm), and safety system override toggles.

---

## 🔬 Mathematical Formulation & Theoretical Derivations

The computational kernel evaluates the coupled non-linear feedback loop between neutron population dynamics, radioactive precursor decay, thermal heat generation, and negative Doppler temperature feedback.

### 1. Point Kinetics Equations with Delayed Neutrons (PRKE)

The continuous time-dependent neutron flux and the delayed precursor concentrations are governed by:

$$
\begin{aligned}
\frac{d\Phi(t)}{dt} &= \frac{\rho(t) - \beta}{\Lambda}\,\Phi(t) + \lambda\, C(t) \\
\frac{dC(t)}{dt} &= \frac{\beta}{\Lambda}\,\Phi(t) - \lambda\, C(t)
\end{aligned}
$$

**Physical variables and standard parameters:**

- $\Phi(t)$: normalized relative neutron flux (directly proportional to the total thermal power output $P(t)$).
- $C(t)$: relative concentration of effective delayed neutron precursor groups.
- $\rho(t)$: net reactivity injected into the nuclear core.
- $\beta = 0.0030$: effective delayed neutron fraction for the $^{232}\text{Th} / ^{233}\text{U}$ fuel cycle.
- $\Lambda = 1.0 \times 10^{-3}\ \text{s}$: mean prompt neutron generation time.
- $\lambda = 0.08\ \text{s}^{-1}$: effective precursor group decay constant.

### 2. Thermal-Hydraulics & Temperature Reactivity Feedback

Core thermal energy production and continuous heat dissipation through the primary heat exchanger loop are solved by a mass-heat balance:

$$
\frac{dT(t)}{dt} = \frac{P_{\text{generation}}(t) - P_{\text{cooling}}(t)}{m \cdot C_p}
$$

$$
\rho_{\text{feedback}}(t) = \alpha_T \cdot \bigl(T(t) - T_{\text{ref}}\bigr)
$$

**Thermal coefficients and reference baselines:**

- $T(t)$: core molten salt bulk temperature in degrees Celsius ($^\circ\text{C}$).
- $P_{\text{generation}}(t) = \Phi(t) \cdot P_{\text{nominal}}$: nuclear fission power generated inside the core.
- $P_{\text{cooling}}(t)$: heat removal rate through the primary loop heat exchangers.
- $\alpha_T = -2.5\ \text{pcm}/^\circ\text{C}$: negative Doppler temperature coefficient of reactivity.
- $T_{\text{ref}} = 650.00\ ^\circ\text{C}$: nominal steady-state baseline temperature.

### 3. Closed-Loop Reactivity Balance & AI PID Control Logic

The net reactivity $\rho(t)$ injected into the solver at time step $t$ combines external disturbances, Doppler feedback, and the active AI guardrail action:

$$
\rho(t) = \rho_{\text{external}}(t) + \rho_{\text{PID}}(t) + \rho_{\text{feedback}}(t)
$$

The AI PID guardrail output is calculated from the temperature error signal $e(t) = T_{\text{ref}} - T(t)$:

$$
\rho_{\text{PID}}(t) = K_p \cdot e(t) + K_i \int_0^t e(\tau)\, d\tau + K_d \frac{de(t)}{dt}
$$

where:

- $K_p = 1.2$: proportional gain (immediate correction proportional to the current error).
- $K_i = 0.05$: integral gain (eliminates steady-state offset over time).
- $K_d = 0.10$: derivative gain (damps transient thermal oscillations).

### Summary of Standard Parameters

| Parameter | Symbol | Value | Description |
|---|---|---|---|
| Delayed neutron fraction | $\beta$ | $0.0030$ | Effective fraction for the $^{232}\text{Th} / ^{233}\text{U}$ cycle |
| Prompt generation time | $\Lambda$ | $1.0 \times 10^{-3}\ \text{s}$ | Mean prompt neutron generation time |
| Precursor decay constant | $\lambda$ | $0.08\ \text{s}^{-1}$ | Effective precursor group decay constant |
| Temperature coefficient | $\alpha_T$ | $-2.5\ \text{pcm}/^\circ\text{C}$ | Negative Doppler feedback |
| Reference temperature | $T_{\text{ref}}$ | $650.00\ ^\circ\text{C}$ | Nominal steady-state baseline |
| Proportional gain | $K_p$ | $1.2$ | PID guardrail |
| Integral gain | $K_i$ | $0.05$ | PID guardrail |
| Derivative gain | $K_d$ | $0.10$ | PID guardrail |

---

## 🧮 4th-Order Runge-Kutta (RK4) Numerical Integration Scheme

To ensure numerical stability across discrete time steps $h$, the C++ engine uses the classical RK4 integration algorithm.

Given the system state vector $\dfrac{dy}{dt} = f(t, y)$, the step $y_{n+1}$ is computed as:

$$
\begin{aligned}
k_1 &= f(t_n,\, y_n) \\
k_2 &= f\!\left(t_n + \tfrac{h}{2},\, y_n + \tfrac{h}{2}\,k_1\right) \\
k_3 &= f\!\left(t_n + \tfrac{h}{2},\, y_n + \tfrac{h}{2}\,k_2\right) \\
k_4 &= f\!\left(t_n + h,\, y_n + h\,k_3\right) \\
y_{n+1} &= y_n + \frac{h}{6}\left(k_1 + 2k_2 + 2k_3 + k_4\right)
\end{aligned}
$$

---

## 🗂️ Project Directory Structure

```text
Toryum-Core-AI/
│
├── index.html         # Interactive Sci-Fi Dashboard & Web Simulation Engine (SVG + Chart.js)
├── main.cpp           # Native C++20 Simulation Kernel (RK4 Physics Solver & PID Logic)
├── CMakeLists.txt     # Cross-platform CMake Build Configuration File
├── Makefile           # Alternative GNU Make automation script
└── README.md          # Comprehensive Project Documentation & Physical Specifications
```

---

## 💻 Local Setup & Execution Guide

### Option 1: Web Dashboard Execution (Live & Local Server)

**Live GitHub Pages site:** <https://kallymeire.github.io/Toryum-Core-AI/>

**Local web server execution:**

```bash
git clone https://github.com/kallymeire/Toryum-Core-AI.git
cd Toryum-Core-AI
python3 -m http.server 8000
```

Navigate to <http://localhost:8000> in any web browser to view the interactive control panel locally.

### Option 2: Native C++20 Core Compilation

To compile and execute the native C++ numerical physics solver locally:

1. Create a build workspace:

   ```bash
   mkdir -p build && cd build
   ```

2. Generate the build scripts and compile using CMake:

   ```bash
   cmake ..
   make
   ```

3. Run the compiled executable binary:

   ```bash
   ./ToryumCoreEngine
   ```

---

## 📈 Engineering Roadmap & Future Extensions

- [x] Initial C++20 RK4 numerical solver setup for PRKE point kinetics and heat transfer.
- [x] Real-time Sci-Fi control dashboard implementation (HTML5/CSS3/JavaScript + SVG).
- [x] Continuous deployment and verification on GitHub Pages.
- [ ] WebAssembly (Wasm) integration to compile `main.cpp` directly for in-browser DOM execution.
- [ ] Expansion to a 6-group delayed neutron precursor decay model.
- [ ] Multi-zone spatial core discretization for non-uniform heat distribution.

---

## 📜 License & Author

Distributed under the **MIT License**. Free for academic, personal, and professional portfolio usage.

Engineered with a focus on cyber-physical systems, software resilience, high-performance C++ architecture, and real-time scientific telemetry visualization.

**Author:** Kallymeire Coelho · [GitHub](https://github.com/kallymeire) · [LinkedIn](https://www.linkedin.com/in/kallymeire-coelho-212746263)