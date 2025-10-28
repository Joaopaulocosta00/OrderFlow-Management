# Sistema de Controle de Pedidos

## Objetivo
Desenvolver um sistema de software para controle de pedidos registrados por um usuário para clientes de uma empresa.

## Metas
* Compreender os requisitos especificados para o sistema de controle de pedidos.
* Definir uma equipe de até três acadêmicos para execução do projeto.
* Elaborar uma matriz de responsabilidades, designando a responsabilidade individual de cada membro da equipe.
* Codificar os módulos **Manter Cliente**, **Manter Produtos** e **Manter Pedidos**.
* Testar os módulos desenvolvidos.
* Apresentar o software ao professor.

## Solução (Tecnologias)
* O software será codificado na linguagem de programação **C**.
* Será utilizada a biblioteca **ncurses** para desenvolvimento de uma interface para o usuário.
* A persistência dos dados será realizada em arquivos texto no formato **CSV** (armazenados na pasta `data/`).
* O sistema será desenvolvido, prioritariamente, para funcionamento na plataforma **Linux**, podendo também ser testado em Microsoft Windows.

## Estrutura do Projeto
O projeto está organizado em módulos que funcionarão como etapas. Cada módulo possui um conjunto de artefatos que descrevem os requisitos e estrutura do sistema de software a ser codificado.

### Artefatos
* Diagrama de caso de uso
* Descrição de cenário de uso
* Diagrama de Estruturas de Dados (`structs`)
* Diagrama de Estrutura Modular
* Descrição requisitos de implementação

## Critérios de Avaliação
O resultado da avaliação deste projeto corresponderá à 4,0 pontos da média referente ao 2º bimestre. Esta avaliação será realizada individualmente e ponderada de acordo com a matriz de responsabilidades definidas pela equipe.

---

## Estrutura de Diretórios
SistemaControlePedidos/ │ ├── include/ ← cabeçalhos (.h) │ ├── cliente.h │ ├── produto.h │ ├── pedido.h │ ├── interface.h │ └── persistencia.h │ ├── src/ ← código-fonte (.c) │ ├── cliente.c │ ├── produto.c │ ├── pedido.c │ ├── interface.c │ ├── persistencia.c │ └── main.c │ ├── data/ ← arquivos CSV para persistência │ ├── Clientes.csv │ ├── Produtos.csv │ └── Pedidos.csv │ ├── build/ ← gerado automaticamente (arquivos objeto e executável) └── Makefile ← arquivo de automação da compilação


### Comandos do Makefile

| Regra | Função |
| :--- | :--- |
| `all` | Regra principal: compila tudo |
| `run` | Compila e executa o projeto |
| `clean` | Remove tudo que foi compilado (limpeza) |
| `help` | Mostra a lista de comandos disponíveis |
| `OBJ_FILES` | Define os arquivos objeto gerados em `build/` |
| `$(EXEC)` | Define o nome do executável (com `.exe` no Windows) |
| `dirs` | Cria a pasta `build` se não existir |
