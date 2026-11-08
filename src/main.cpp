#include <iostream>

struct ReactorCoreState {
    double th232_mass_kg = 1200.0;
    double u235_mass_kg  = 150.0;
    double pu239_mass_kg = 45.0;
    double core_temp_celsius = 650.0;
    bool system_nominal = true;
};

void print_system_telemetry(const ReactorCoreState& state) {
    std::cout << "\n============================================\n";
    std::cout << "        TORYUM CORE AI - KERNEL v0.1.0      \n";
    std::cout << "============================================\n";
    std::cout << " [STATUS]: " << (state.system_nominal ? "NOMINAL (SAFE)" : "CRITICAL") << "\n";
    std::cout << " [NÚCLEO]: Temperatura: " << state.core_temp_celsius << " °C\n";
    std::cout << " -------------------------------------------\n";
    std::cout << " [INVENTÁRIO DE COMBUSTÍVEL NEUTRÔNICO]:\n";
    std::cout << "  * Th-232 (Fértil Base)       : " << state.th232_mass_kg << " kg\n";
    std::cout << "  * U-235/233 (Gatilho Físsil) : " << state.u235_mass_kg << " kg\n";
    std::cout << "  * Pu-239 (Co-Gerador)        : " << state.pu239_mass_kg << " kg\n";
    std::cout << "============================================\n";
}

int main() {
    std::cout << "[*] Inicializando a infraestrutura do Toryum Core AI...\n";
    ReactorCoreState core;
    print_system_telemetry(core);
    std::cout << "[*] Teste de compilação C++20 via CMake concluído com sucesso.\n";
    return 0;
}