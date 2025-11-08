# 🧠 Project Bart — Software Architecture Document (v0.1)

## 1. Introdução

O **Project Bart** é um rover modular desenvolvido para o **European Rover Challenge**, com foco em navegação semiautónoma, recolha de dados científicos e execução de tarefas de exploração.

Este documento define a **arquitetura de software** do sistema, baseada em **ROS 2 (Humble ou Iron)**, e serve como referência para o desenvolvimento, integração e simulação do rover.

---

## 2. Visão Geral do Sistema

O sistema de software do Bart é composto por vários **módulos ROS 2**, cada um responsável por uma parte da operação do rover.  
A arquitetura segue uma estrutura modular e distribuída, permitindo substituição e testes independentes dos componentes.

### 🧩 Diagrama de Blocos (Resumo)

[Sensores]
   ↓
[LIDAR Node] → [Perception Node] → [Navigation Node] → [Motor Control Node]
                                      ↓
                                  [Telemetry Node] → [CLI/UI Interface]

---

## 3. Componentes Principais

| Node | Descrição | Tipo de Comunicação |
|------|------------|--------------------|
| `lidar_node` | Lê dados do LIDAR e publica nuvem de pontos / varrimento. | Publica `/scan` |
| `imu_node` | Publica dados de orientação e aceleração. | Publica `/imu` |
| `perception_node` | Processa dados de sensores (fusão de sensores, detecção de obstáculos). | Subscreve `/scan`, `/imu`; publica `/obstacles` |
| `navigation_node` | Responsável por planeamento de trajetórias e controlo de movimento. | Subscreve `/obstacles`; publica `/cmd_vel` |
| `motor_control_node` | Converte comandos de velocidade em sinais para motores (PWM, CAN, etc.). | Subscreve `/cmd_vel` |
| `telemetry_node` | Agrega dados e envia telemetria para o sistema remoto. | Publica `/status`, `/diagnostics` |
| `ui_node` (ou CLI) | Interface para operadores — envia comandos e recebe estado. | Usa serviços e tópicos |

---

## 4. Comunicação ROS 2

### 📡 Tópicos Principais

| Tópico | Tipo de Mensagem | Publicador | Subscritor |
|--------|------------------|-------------|-------------|
| `/scan` | `sensor_msgs/LaserScan` | `lidar_node` | `perception_node`, `navigation_node` |
| `/imu` | `sensor_msgs/Imu` | `imu_node` | `perception_node` |
| `/obstacles` | `bart_msgs/ObstacleArray` | `perception_node` | `navigation_node` |
| `/cmd_vel` | `geometry_msgs/Twist` | `navigation_node` | `motor_control_node` |
| `/odom` | `nav_msgs/Odometry` | `motor_control_node` | `navigation_node`, `telemetry_node` |
| `/status` | `bart_msgs/SystemStatus` | `telemetry_node` | `ui_node` |

### 🧩 Serviços Planeados

| Serviço | Descrição | Tipo de Mensagem |
|----------|------------|------------------|
| `/set_waypoint` | Define um novo destino para o rover. | `geometry_msgs/Pose` |
| `/get_status` | Retorna estado atual do sistema. | `bart_msgs/SystemStatus` |

---

## 5. Camadas de Software

1. **Perception Layer** → LIDAR, IMU, Sensor Fusion  
2. **Planning Layer** → Path Planning, Obstacle Avoidance  
3. **Control Layer** → Motion control, PID, Odometry  
4. **Interface Layer** → CLI, Web UI, Telemetry  

Cada camada comunica via tópicos ROS 2 e mantém baixo acoplamento entre módulos.

---

## 6. Execução e Lançamento

### Estrutura de Packages
```
bart_software/
├── bart_perception/
├── bart_navigation/
├── bart_control/
├── bart_telemetry/
├── bart_interfaces/   # Mensagens e serviços ROS 2
├── bart_simulation/   # Mundo Gazebo + launch files
└── bart_ui/           # CLI ou interface web
```

### Launch Example
```bash
ros2 launch bart_simulation sim.launch.py
```

Este comando inicializa todos os nós simulados, sensores mock e o modelo do rover no Gazebo.

---

## 7. Simulação e Testes

- **Gazebo** será usado para simular sensores e terreno.
- **RViz 2** para visualização e debug de dados.  
- Módulos “mock” substituem sensores reais durante o desenvolvimento.
- Testes automatizados com `pytest` e `ros2 launch test`.

---

## 8. Planos Futuros

- Integração de **câmera e visão por computador** (detecção de marcadores).  
- Suporte para **autonomia completa com SLAM**.  
- Módulo de **drone** (opcional) para mapeamento aéreo.  
- Expansão do sistema de diagnóstico remoto e telemetria.

---

📘 *Última atualização:* Novembro 2025  
👤 *Autor:* Diogo Pinheiro — Software Leader, Project Bart
