# Atividade 04 - Sincronização de Tempo com SNTP e ZBus

## Descrição

Sistema embarcado que sincroniza o relógio interno com um servidor SNTP e distribui atualizações de tempo para diferentes módulos usando o ZBus (Event Bus) do Zephyr RTOS.

## Arquitetura do Sistema

O sistema é dividido em três componentes principais que se comunicam através do canal ZBus `time_channel`:

### 1. Thread SNTP Client (`sntp_client.c`)
- **Responsabilidade**: Sincronização periódica com servidor SNTP
- **Funcionalidades**:
  - Conecta-se ao servidor SNTP configurado
  - Sincroniza o relógio do sistema
  - Publica atualizações de tempo no canal ZBus
  - Intervalo de sincronização configurável via Kconfig

### 2. Thread Logger (`logger_module.c`)
- **Responsabilidade**: Registro de timestamps
- **Funcionalidades**:
  - Subscreve ao canal `time_channel`
  - Atualiza relógio interno quando recebe novos timestamps
  - Grava logs com informações de data/hora
  - Emite heartbeat periódico

### 3. Thread Application (`app_module.c`)
- **Responsabilidade**: Processamento de eventos baseado em tempo
- **Funcionalidades**:
  - Subscreve ao canal `time_channel`
  - Calcula intervalos entre eventos
  - Processa eventos periódicos
  - Agenda operações futuras

## Estrutura de Arquivos

```
src/
├── time_common.h         # Estruturas de dados e definições compartilhadas
├── sntp_client.c/h       # Implementação do cliente SNTP
├── logger_module.c/h     # Implementação do módulo de logging
├── app_module.c/h        # Implementação do módulo de aplicação
└── main.c                # Inicialização do sistema

boards/
└── qemu_cortex_m3.conf   # Configurações de rede para simulação
```

## Configuração

### Servidor SNTP

Pode ser configurado através do arquivo `Kconfig`:

```kconfig
config SNTP_SERVER
    string "SNTP server hostname"
    default "pool.ntp.org"
```

### Intervalo de Sincronização

```kconfig
config SNTP_SYNC_INTERVAL
    int "SNTP synchronization interval (seconds)"
    default 60
    range 10 3600
```

## ZBus - Canal de Comunicação

### Estrutura de Mensagem

```c
struct time_msg {
    int64_t timestamp;       // Unix timestamp em segundos
    int64_t uptime_ms;       // System uptime em milissegundos
    bool is_synchronized;    // Se o tempo foi sincronizado com SNTP
    struct tm time_info;     // Informação de tempo formatada
};
```

### Canal ZBus

```c
ZBUS_CHAN_DEFINE(time_channel,
                 struct time_msg,
                 NULL,
                 NULL,
                 ZBUS_OBSERVERS_EMPTY,
                 ZBUS_MSG_INIT(0));
```

### Observers

- **Logger Listener**: Registra timestamps recebidos
- **Application Listener**: Processa eventos baseados em tempo

## Compilação e Execução

### Compilar

```bash
west build -b qemu_cortex_m3 -p
```

### Executar no QEMU

```bash
west build -t run
```

## Funcionalidades Implementadas

### ✅ Sincronização SNTP
- Conexão com servidor SNTP
- Atualização do relógio do sistema
- Retry automático em caso de falha
- Logs detalhados de sincronização

### ✅ ZBus Publisher/Subscriber
- Canal de mensagens `time_channel`
- Publisher: SNTP Client
- Subscribers: Logger e Application
- Comunicação assíncrona entre módulos

### ✅ Logger Module
- Registro de timestamps
- Logs estruturados com data/hora
- Heartbeat periódico
- Relógio interno atualizado

### ✅ Application Module
- Cálculo de intervalos de tempo
- Processamento de eventos periódicos
- Rastreamento de tempo entre eventos
- Agendamento baseado em tempo

### ✅ Configuração via Kconfig
- Servidor SNTP configurável
- Intervalo de sincronização ajustável
- Parâmetros de rede customizáveis

## Saída Esperada

```
===========================================
Sistema Embarcado - Atividade 04
SNTP + ZBus - Sincronização de Tempo
===========================================

[SNTP Client] Iniciando sincronização SNTP...
[SNTP Client] Sincronização SNTP bem-sucedida
[SNTP Client] Horário publicado: 2025-11-02 14:30:15 UTC

[Logger] === LOG ENTRY ===
[Logger] Timestamp: 1730558415
[Logger] Data/Hora: 2025-11-02 14:30:15 UTC
[Logger] Uptime: 2500 ms
[Logger] Status: Relógio sincronizado com SNTP
[Logger] ================

[Application] === APPLICATION EVENT ===
[Application] Horário recebido: 2025-11-02 14:30:15 UTC
[Application] Tempo desde última atualização: 60 segundos
[Application] ========================

[Application] Processando evento periódico #1
[Application] >>> Evento da aplicação processado
[Application] >>> Timestamp do evento: 1730558415
```

## Requisitos Técnicos Atendidos

- [x] Utilização correta do ZBus com publisher/subscribers
- [x] Organização modular entre as threads
- [x] Responsabilidades bem definidas para cada componente
- [x] Uso do SNTP para sincronização com servidor externo
- [x] Configuração do servidor SNTP via Kconfig
- [x] Publicação apenas quando sincronização bem-sucedida
- [x] Logs claros de atualização e sincronização

## Observações

- O sistema continua executando as threads da Atividade 03 (sensores/filtro/processadora)
- A rede é simulada via SLIP em ambiente QEMU
- Para produção, ajuste as configurações de rede e servidor SNTP
- Os timestamps são em formato UTC

## Referências

- [Zephyr SNTP Documentation](https://docs.zephyrproject.org/latest/connectivity/networking/api/sntp.html)
- [Zephyr ZBus Documentation](https://docs.zephyrproject.org/latest/services/zbus/index.html)
- [Zephyr Networking Guide](https://docs.zephyrproject.org/latest/connectivity/networking/index.html)
