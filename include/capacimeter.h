#ifndef CAPACIMETER_H
#define CAPACIMETER_H

#include <Arduino.h>

// Mapeamento de hardware
constexpr uint8_t TRIG_PIN = 9;
constexpr uint8_t OUT_PIN = 8; // Requisito de hardware: ICP1 no ATmega328P

// Parâmetros operacionais e constantes matemáticas
constexpr float RA_OHMS = 12000.0;
constexpr float F_CLK = 16000000.0;
constexpr float CONST_LN3 = 1.098612;
constexpr float R_ON_555 = 50.0;
constexpr uint32_t TRIGGER_PULSE_US = 12;

/**
 * @brief Inicializa os periféricos, portas e configuração serial do microcontrolador.
 */
void initCapacimeter();

/**
 * @brief Gera o pulso de disparo negativo para o terminal TRIG do temporizador 555.
 */
void triggerPulse();

/**
 * @brief Monitora as bordas do sinal e captura a duração temporal via Timer1.
 * @return Número total de incrementos de tempo acumulados (ticks).
 */
uint32_t processTimeCapture();

/**
 * @brief Converte a contagem digital de tempo para o valor físico da capacitância.
 * @param totalTicks Contagem bruta registrada pelo temporizador.
 * @return Capacitância calculada em Farads.
 */
float calculateCapacitance(uint32_t totalTicks);

/**
 * @brief Aplica o atraso de software necessário para a descarga completa do capacitor.
 * @param capacitance Valor da capacitância aferida na última leitura.
 */
void waitForDeadTime(float capacitance);

#endif