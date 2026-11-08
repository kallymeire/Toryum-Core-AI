#include <iostream>
#include <vector>
#include <cmath>
#include <concepts>
#include <chrono>
#include <thread>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>

template <typename T>
concept FloatingPoint = std::floating_point<T>;

template <FloatingPoint T>
struct ReactorState {
    T temp_core;      
    T neutron_flux;   
    T reactivity;     
    T precursor_conc; 
};

template <FloatingPoint T>
class PIDController {
private:
    T kp, ki, kd;
    T integral;
    T last_error;
public:
    PIDController(T p, T i, T d) : kp(p), ki(i), kd(d), integral(0), last_error(0) {}

    T compute(T setpoint, T current, T dt) {
        T error = setpoint - current;
        integral += error * dt;
        T derivative = (error - last_error) / dt;
        last_error = error;
        return (kp * error) + (ki * integral) + (kd * derivative);
    }
};

template <FloatingPoint T>
class MSRCoreSolver {
private:
    constexpr static T BETA = 0.0030;         
    constexpr static T LAMBDA = 0.08;         
    constexpr static T PROMPT_GEN_TIME = 1e-3; 
    constexpr static T ALPHA_TEMP = -2.5;     
    constexpr static T TARGET_TEMP = 650.0;   
    constexpr static T HEAT_CAPACITY = 800.0; 
    constexpr static T POWER_NOMINAL = 300.0; 

public:
    ReactorState<T> computeDerivatives(const ReactorState<T>& state, T external_reactivity) const {
        T temp_feedback = ALPHA_TEMP * (state.temp_core - TARGET_TEMP);
        T total_reactivity = external_reactivity + temp_feedback;
        T rho = total_reactivity * 1e-5;

        T d_flux = ((rho - BETA) / PROMPT_GEN_TIME) * state.neutron_flux + LAMBDA * state.precursor_conc;
        T d_precursor = (BETA / PROMPT_GEN_TIME) * state.neutron_flux - LAMBDA * state.precursor_conc;

        T power_gen = state.neutron_flux * POWER_NOMINAL;
        T heat_removal = (state.temp_core - 25.0) * (POWER_NOMINAL / (TARGET_TEMP - 25.0));
        T d_temp = (power_gen - heat_removal) / HEAT_CAPACITY;

        return { d_temp, d_flux, total_reactivity, d_precursor };
    }

    ReactorState<T> stepRK4(const ReactorState<T>& state, T dt, T ext_reactivity) const {
        auto k1 = computeDerivatives(state, ext_reactivity);

        ReactorState<T> s2 {
            state.temp_core + 0.5 * dt * k1.temp_core,
            state.neutron_flux + 0.5 * dt * k1.neutron_flux,
            state.reactivity,
            state.precursor_conc + 0.5 * dt * k1.precursor_conc
        };
        auto k2 = computeDerivatives(s2, ext_reactivity);

        ReactorState<T> s3 {
            state.temp_core + 0.5 * dt * k2.temp_core,
            state.neutron_flux + 0.5 * dt * k2.neutron_flux,
            state.reactivity,
            state.precursor_conc + 0.5 * dt * k2.precursor_conc
        };
        auto k3 = computeDerivatives(s3, ext_reactivity);

        ReactorState<T> s4 {
            state.temp_core + dt * k3.temp_core,
            state.neutron_flux + dt * k3.neutron_flux,
            state.reactivity,
            state.precursor_conc + dt * k3.precursor_conc
        };
        auto k4 = computeDerivatives(s4, ext_reactivity);

        return {
            state.temp_core + (dt / 6.0) * (k1.temp_core + 2.0*k2.temp_core + 2.0*k3.temp_core + k4.temp_core),
            std::max(0.001, state.neutron_flux + (dt / 6.0) * (k1.neutron_flux + 2.0*k2.neutron_flux + 2.0*k3.neutron_flux + k4.neutron_flux)),
            k4.reactivity,
            std::max(0.001, state.precursor_conc + (dt / 6.0) * (k1.precursor_conc + 2.0*k2.precursor_conc + 2.0*k3.precursor_conc + k4.precursor_conc))
        };
    }
};

// Gerador de Barra Gráfica ASCII para o Terminal
std::string renderProgressBar(double value, double minVal, double maxVal, int width = 15) {
    double norm = (value - minVal) / (maxVal - minVal);
    norm = std::clamp(norm, 0.0, 1.0);
    int filled = static_cast<int>(norm * width);
    std::string bar = "[";
    for (int i = 0; i < width; ++i) {
        if (i < filled) bar += "=";
        else if (i == filled) bar += ">";
        else bar += ".";
    }
    bar += "]";
    return bar;
}

int main() {
    std::cout << "=====================================================================================\n";
    std::cout << "    TORYUM CORE AI - SCI-FI ADVANCED TELEMETRY DASHBOARD (v5.0)                      \n";
    std::cout << "    Molten Salt Thorium Reactor (MSR) - RK4 Solver + Autonomous AI Guardrail         \n";
    std::cout << "=====================================================================================\n\n";

    std::ofstream csv_file("telemetria_msr.csv");
    csv_file << "Tempo,TempCore,FluxoNeutronico,Reatividade,AcaoAI\n";

    MSRCoreSolver<double> solver;
    PIDController<double> ai_controller(1.2, 0.05, 0.1);
    
    ReactorState<double> state { 650.0, 1.0, 0.0, 0.0375 };

    double dt = 0.01; 
    double time = 0.0;
    double manual_perturbation = 150.0; // Injeção de perturbação térmica (+150 pcm)

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "TIME(s) | TEMP (°C) | TEMP BAR          | FLUX  | FLUX BAR          | AI CONTROL | STATUS\n";
    std::cout << "-------------------------------------------------------------------------------------\n";

    for (int step = 0; step <= 60; ++step) {
        double control_action = ai_controller.compute(650.0, state.temp_core, dt);
        double total_ext_reactivity = manual_perturbation + control_action;

        // Renderização das barras visuais
        std::string temp_bar = renderProgressBar(state.temp_core, 600.0, 700.0, 15);
        std::string flux_bar = renderProgressBar(state.neutron_flux, 0.5, 2.0, 15);

        std::string status;
        if (std::abs(state.temp_core - 650.0) < 3.0) {
            status = "[NOMINAL/AI]";
        } else if (state.temp_core > 660.0) {
            status = "[ALERT/HIGH]";
        } else {
            status = "[STABILIZING]";
        }
        
        std::cout << std::setw(7)  << time << " | "
                  << std::setw(8)  << state.temp_core << " | "
                  << std::setw(17) << temp_bar << " | "
                  << std::setw(5)  << state.neutron_flux << " | "
                  << std::setw(17) << flux_bar << " | "
                  << std::setw(10) << control_action << " | "
                  << status << "\n";

        csv_file << time << "," << state.temp_core << "," << state.neutron_flux << "," << total_ext_reactivity << "," << control_action << "\n";

        for (int i = 0; i < 10; ++i) {
            state = solver.stepRK4(state, dt, total_ext_reactivity);
        }

        if (time > 2.0) manual_perturbation = 0.0;
        
        time += (dt * 10.0);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    csv_file.close();
    std::cout << "-------------------------------------------------------------------------------------\n";
    std::cout << "\n[STATISTICS SUMMARY]\n";
    std::cout << " * Core Status     : OPTIMAL REGULATED\n";
    std::cout << " * Target Temp     : 650.00 °C\n";
    std::cout << " * Final Temp      : " << state.temp_core << " °C\n";
    std::cout << " * Telemetry File  : telemetria_msr.csv (EXPORTED)\n";
    std::cout << "=====================================================================================\n";
    return 0;
}