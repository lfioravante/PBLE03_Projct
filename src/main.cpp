#include <Arduino.h>
#include "capacimeter.h"

void setup() {
    initCapacimeter();
}

void loop() {
    triggerPulse();
    
    uint32_t computedTicks = processTimeCapture();
    float capacitanceValue = calculateCapacitance(computedTicks);
    
    Serial.print("Contagem Total (N): ");
    Serial.print(computedTicks);
    Serial.print(" | Capacitância Estimada (F): ");
    Serial.println(capacitanceValue, 10);
    
    waitForDeadTime(capacitanceValue);
    
    // Intervalo de leitura entre ciclos
    delay(500); 
}