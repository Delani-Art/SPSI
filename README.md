# Sistema de Processamento de Sensores Industriais

## Descrição do Projeto
Este projeto implementa um sistema em linguagem C para processamento de dados de sensores industriais, dividido em três programas:

- **Programa 1 - organiza_dados.c:**
  - Processa um arquivo bruto (`teste.txt`) com leituras de sensores.
  - Separa os dados por sensor.
  - Ordena por timestamp em **ordem decrescente**.
  - Gera arquivos `.dat` individuais por sensor.

- **Programa 2 - consulta_sensor.c:**  
  - Permite consultar a leitura mais próxima de um **timestamp específico**.
  - Usa **busca binária em arquivos decrescentes** para alto desempenho.
  - Exibe o timestamp e também a **data legível da leitura**.

- **Programa 3 - gera_teste.c:**  
  - Gera um arquivo de teste com 2000 leituras para cada sensor informado.
  - Timestamps são aleatórios dentro de um intervalo definido pelo usuário.
  - Valores são gerados conforme o tipo de sensor (`CONJ_Z`, `BINARIO`, etc.).

---

## Como Compilar e Executar

  ### Pré-requisitos
    Ter o compilador `gcc` instalado na máquina.

  ### Compilação
    Abra o terminal na pasta do projeto e execute:

    gcc organiza_dados.c utils.c -o organiza
    gcc consulta_sensor.c utils.c -o consulta
    gcc gera_teste.c utils.c -o teste

  ### Execução
  - Comece pelo programa de teste que vai gerar um arquivo chamado "teste.txt" 
    Exemplo comparativo dos parametros:
    
      .\teste.exe 01/06/2024 15/06/2024 temp01 CONJ_Q press01 CONJ_Z door01 BINARIO cod01 TEXTO...
      .\teste.exe <data_inicial> <data_final> <sensor1> <tipo1> <sensor2> <tipo2> <sensor3> <tipo3> <sensor4> <tipo4>...

  | Parâmetros       | O que é                                           | Exemplo                                |
  | ---------------- | ------------------------------------------------- | -------------------------------------- |
  | `<data_inicial>` | Data de início das leituras (formato: DD/MM/AAAA) | `01/06/2024`                           |
  | `<data_final>`   | Data de fim das leituras (formato: DD/MM/AAAA)    | `15/06/2024`                           |
  | `<sensorX>`      | Nome de um sensor sem espaços                     | `temp01`, `door01`                     |
  | `<tipoX>`        | Tipo de dado gerado por esse sensor               | `CONJ_Z`, `CONJ_Q`, `BINARIO`, `TEXTO` |

  | Tipo      | Descrição       | Valor gerado         |
  | --------- | --------------- | -------------------- |
  | `CONJ_Z`  | Número inteiro  | `42`, `7`, `99`      |
  | `CONJ_Q`  | Número racional | `3.14`, `99.00`      |
  | `BINARIO` | Booleano        | `true`, `false`      |
  | `TEXTO`   | Texto (string)  | `cod0012`, `cod8237` |

  - Após criar o teste.txt, execute o programa para organizar os timestamps em ordem decrescente.
  - serão criados x arquivos de acordo com quantos sensores você havia solicitado.
  - agora pode usar o programa para consultar um timestamp e ver se está funcionando corretamente.
