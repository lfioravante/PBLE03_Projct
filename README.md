Representação estruturada em linguagem Mermaid do fluxograma lógico do *firmware* para o capacimetro:

```mermaid
flowchart TD
    Start([Início]) --> Init[Inicializar Serial, GPIOs e limpar Timer1]
    Init --> Trigger[Gerar Pulso em TRIG: Nível Baixo por 12 µs]
    Trigger --> StartTimerRise[Iniciar Timer1 e configurar Captura para Borda de Subida]
    StartTimerRise --> WaitRise{Flag ICF1 acionada?}
    WaitRise -- Não --> WaitRise
    WaitRise -- Sim --> ConfigFall[Zerar TCNT1, limpar flags e reconfigurar para Borda de Descida]
    ConfigFall --> WaitFall{Flag ICF1 acionada?}
    WaitFall -- Não --> CheckOVF{Flag TOV1 acionada?}
    CheckOVF -- Sim --> IncOVF[Incrementar overflowCount e limpar flag TOV1]
    IncOVF --> WaitFall
    CheckOVF -- Não --> WaitFall
    WaitFall -- Sim --> Capture[Salvar registrador ICR1 e parar Timer1]
    Capture --> RaceCondition{Flag TOV1 ativa e ICR1 < 32768?}
    RaceCondition -- Sim --> IncOVFRace[Incrementar overflowCount]
    IncOVFRace --> CalcTicks
    RaceCondition -- Não --> CalcTicks[Calcular totalTicks = overflowCount * 65536 + ICR1]
    CalcTicks --> CalcCap[Calcular Capacitância: C = totalTicks / - RA * f_clk * ln3 -]
    CalcCap --> Display[Enviar resultados via porta Serial]
    Display --> Delay[Aguardar tempo de descarga do capacitor e atraso de 500 ms]
    Delay --> Trigger

```