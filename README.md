# OrderFlow-Management


Objetivo
Desenvolver um sistema de software para controle de pedidos registrados por um usuário para clientes
de uma empresa.

Metas
Compreender os requisitos especificados para o sistema de controle de pedidos. Definir uma equipe de
até três acadêmicos para execução do projeto. 
Elaborar uma matriz de responsabilidades, designando a responsabilidade individual de cada membro da equipe.
Codificar os módulos Manter Cliente, Manter
Produtos e Manter Pedidos. 
Testar os módulos desenvolvidos. 
Apresentar o software ao professor.

Solução
O software deve ser codificado na linguagem de programação C. Será utilizada a biblioteca ncurses para
desenvolvimento de uma interface para o usuário. A persistência dos dados mantidos pelo sistema será
realizada em arquivos texto no formato CSV. O sistema de software será desenvolvido, prioritariamente, para
funcionamento na plataforma Linux, podendo também ser testado em Microsoft Windows.
Estrutura do Projeto

O projeto está organizado em módulos, que funcionarão como etapas do mesmo. Cada módulo possui um
conjunto de artefatos que descrevem os requisitos e estrutura do sistema de software a ser codificado.
Artefatos:

• Diagrama de caso de uso
• Descrição de cenário de uso
• Diagrama de Estruturas de Dados (structs)
• Diagrama de Estrutura Modular
• Descrição requisitos de implementação
Critérios de Avaliação
O resultado da avaliação deste projeto corresponderá à 4,0 pontos da média referente ao 2o bimestre*. Esta
avaliação será realizada individualmente e ponderada de acordo com a matriz de responsabilidades
definidas pela equipe.

Estrutura final do projeto
SistemaControlePedidos/
│
├── include/ ← cabeçalhos (.h)
│ ├── cliente.h
│ ├── produto.h
│ ├── pedido.h
│ ├── interface.h
│ └── persistencia.h
│
├── src/ ← código-fonte (.c)
│ ├── cliente.c
│ ├── produto.c
│ ├── pedido.c
│ ├── interface.c
│ ├── persistencia.c
│ └── main.c
OBJ_FILES Define os arquivos objeto gerados em build/
$(EXEC) Define o nome do executável (com .exe no Windows)
all Regra principal: compila tudo
dirs Cria a pasta build se não existir
run Compila e executa
clean Remove tudo que foi compilado
help Mostra a lista de comandos disponíveis
Linha Função

│
├── data/ ← arquivos CSV
│ ├── Clientes.csv
│ ├── Produtos.csv
│ └── Pedidos.csv
│
├── build/ ← gerado automaticamente
│
└── Makefile ← este arquivo
