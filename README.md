# Simulador Configurável de Temperatura com Geração de Relatórios

## Descrição do Projeto

Este projeto consiste em um **simulador de sensor de temperatura** desenvolvido para a plataforma **BitDogLab** utilizando a **Raspberry Pi Pico**. O sistema permite a simulação de leituras de temperatura, configuração de parâmetros operacionais e geração de relatórios detalhados via comunicação UART. O objetivo principal é fornecer uma ferramenta para testes e validação de sistemas embarcados sem a necessidade de sensores físicos, facilitando o desenvolvimento e a experimentação.

## Funcionalidades Principais

- **Simulação de Temperatura**: Utiliza o joystick integrado para variar os valores de temperatura simulados.
- **Configuração Personalizável**: Permite ajustes de limites de temperatura, unidades de medida (Celsius, Fahrenheit) e outros parâmetros através de um menu interativo.
- **Geração de Relatórios**: Registra eventos e variáveis do sistema, exportando os dados via UART para análise posterior.
- **Feedback Visual e Sonoro**: Indicação de estados do sistema por meio de LEDs RGB, matriz de LEDs e buzzer para alertas.

## Estrutura do Projeto

A organização dos diretórios e arquivos é a seguinte:

- **`/src`**: Contém os arquivos-fonte principais em C, incluindo a lógica do simulador e funções auxiliares.
- **`/include`**: Abriga os arquivos de cabeçalho (`.h`) com declarações de funções e definições de macros.
- **`/Relatórios`**: Diretório destinado ao armazenamento dos relatórios gerados durante a execução do sistema.
- **`main.c`**: Arquivo principal que contém a função `main` e orquestra a inicialização e o loop principal do programa.
- **`CMakeLists.txt`**: Arquivo de configuração do CMake para gerenciamento da construção do projeto.

## Requisitos de Hardware e Software

- **Hardware**:
  - Placa **BitDogLab** com **Raspberry Pi Pico** integrada.
  - Componentes integrados: joystick, LEDs RGB, matriz de LEDs, display OLED e buzzer.

- **Software**:
  - Ambiente de desenvolvimento **Visual Studio Code** com extensão para Raspberry Pi Pico.
  - **CMake** para gerenciamento de build.
  - **SDK do Raspberry Pi Pico** configurado no ambiente.

## Instruções de Compilação e Execução

1. **Clonar o Repositório**:
   ```bash
   git clone https://github.com/Marques-svnt/Projeto_Final.git
   ```

2. **Configurar o Ambiente**:
   - Certifique-se de que o SDK do Raspberry Pi Pico esteja corretamente instalado e configurado.
   - Instale o CMake e o Visual Studio Code, caso ainda não estejam disponíveis.

3. **Compilar o Projeto**:
   - Abra o terminal no diretório raiz do projeto.
   - Crie uma pasta para os arquivos de build:
     ```bash
     mkdir build
     cd build
     ```
   - Configure o projeto com o CMake:
     ```bash
     cmake ..
     ```
   - Compile o código:
     ```bash
     make
     ```

4. **Carregar o Firmware**:
   - Conecte a Raspberry Pi Pico ao computador em modo de armazenamento USB.
   - Copie o arquivo `.uf2` gerado na pasta `build` para o dispositivo de armazenamento correspondente à Pico.

5. **Interagir com o Sistema**:
   - Utilize um monitor serial (como o integrado no Visual Studio Code) para visualizar os logs e interagir com o sistema via UART.
   - Navegue pelos menus e configure os parâmetros conforme necessário utilizando os botões e o joystick da BitDogLab.

## Link do vídeo: https://www.youtube.com/watch?v=Qm6BEPYr_8s

## Licença

Este projeto está licenciado sob a licença MIT. Consulte o arquivo `LICENSE` para mais informações.

