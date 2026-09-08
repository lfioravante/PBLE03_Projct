#include "capacimeter.h"

void initCapacimeter() {
    Serial.begin(9600);
    
    pinMode(TRIG_PIN, OUTPUT);
    digitalWrite(TRIG_PIN, HIGH);
    pinMode(OUT_PIN, INPUT);

    // Garante que o Timer1 inicie desativado e em modo normal de contagem
    TCCR1A = 0;
    TCCR1B = 0;
}

void triggerPulse() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(TRIGGER_PULSE_US);
    digitalWrite(TRIG_PIN, HIGH);
}

uint32_t processTimeCapture() {
    uint16_t overflowCount = 0;
    uint16_t finalCaptureRegister = 0;

    // Limpa as sinalizações (flags) de interrupção pendentes
    TIFR1 = (1 << ICF1) | (1 << TOV1);
    
    // Inicia o Timer1 sem divisão de frequência e aguarda a borda de subida
    TCCR1B = (1 << ICES1) | (1 << CS10);

    // Malha de espera ativa (polling) para o início do pulso
    while ((TIFR1 & (1 << ICF1)) == 0) {
        // Aguarda a transição inicial
    }

    // Prepara o registrador para iniciar a medição exata do pulso
    TCNT1 = 0;
    TIFR1 = (1 << ICF1) | (1 << TOV1);
    
    // Altera a configuração do módulo para detectar a borda de descida
    TCCR1B &= ~(1 << ICES1);

    // Malha de espera ativa para o término do pulso
    while ((TIFR1 & (1 << ICF1)) == 0) {
        // Monitoramento de transbordo (overflow) do temporizador de 16 bits
        if ((TIFR1 & (1 << TOV1)) != 0) {
            overflowCount++;
            TIFR1 = (1 << TOV1); 
        }
    }

    // Leitura do valor retido no instante exato da borda de descida
    finalCaptureRegister = ICR1;

    // Tratamento preventivo para condições de corrida no instante da captura
    if ((TIFR1 & (1 << TOV1)) != 0 && finalCaptureRegister < 32768) {
        overflowCount++;
    }

    // Interrompe a contagem do temporizador
    TCCR1B = 0;

    return ((uint32_t)overflowCount * 65536UL) + finalCaptureRegister;
}

float calculateCapacitance(uint32_t totalTicks) {
    return (float)totalTicks / (RA_OHMS * F_CLK * CONST_LN3);
}

void waitForDeadTime(float capacitance) {
    float dischargeTimeSeconds = 5.0 * R_ON_555 * capacitance;
    uint32_t dischargeTimeMilliseconds = (uint32_t)(dischargeTimeSeconds * 1000.0);
    
    if (dischargeTimeMilliseconds < 1) {
        dischargeTimeMilliseconds = 1;
    }
    
    delay(dischargeTimeMilliseconds);
}