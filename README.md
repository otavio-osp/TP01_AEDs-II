# TP01_AEDs-II
TRABALHO PRATICO 1 - ALGORITMOS E ESTRUTURAS DE DADOS II

SISTEMA DE BUSCA DE FABULAS (INDICES INVERTIDOS: PATRICIA E HASH)
DESENVOLVEDORES:
- Otávio Soares Pedroso - 6593 (I/O e Pre-processamento)
- Pedro Mendes Gonzaga-6563 (TAD Hash)
- Gabriel Marcus de Oliveira Félix (TAD Patricia)
- Tomás Viana Resende-6556 (Motor de Busca e Integracao)

1. DESCRICAO DO PROGRAMA
O programa e uma maquina de busca para uma biblioteca de fabulas. 
Ele realiza a leitura de documentos de texto, processa as palavras 
(removendo acentuacoes e stop words) e constroi indices invertidos 
usando duas estruturas de dados: uma Tabela Hash (com encadeamento) 
e uma Arvore PATRICIA. O usuario pode buscar por palavras e o sistema 
retornara as fabulas mais relevantes baseadas no calculo TF-IDF.

2. PRE-REQUISITOS
Para compilar e executar este programa, voce precisara de:
- Um compilador C (GCC).
- O utilitario 'make' instalado (MinGW para Windows ou CMake/Make 
  para Linux).

3. COMO COMPILAR
Este projeto utiliza um arquivo Makefile para automatizar a compilacao.
1. Abra o terminal e navegue ate a pasta raiz do projeto.
2. Digite o seguinte comando e pressione Enter:
   make
3. O compilador ira gerar um executavel chamado 'tp01_busca'.

4. COMO EXECUTAR
Apos a compilacao, voce pode executar o programa de duas maneiras:
- Utilizando o proprio makefile com o comando:
  make run
- Ou chamando o executavel diretamente no terminal:
  ./tp01_busca   (no Linux/Mac)
  tp01_busca.exe (no Windows)

5. COMO UTILIZAR O SISTEMA
Ao iniciar o programa, um menu interativo sera exibido. Para o 
funcionamento correto, siga a ordem logica das opcoes:

- OPCAO 1: Le o arquivo de entrada ("entrada.txt"). O arquivo deve estar 
  na mesma pasta do executavel. A primeira linha deve conter o numero 'N' 
  de documentos, seguida pelos nomes dos arquivos das fabulas, um por linha.
- OPCAO 2: Constroi os indices invertidos em memoria, inserindo as 
  palavras na Tabela Hash e na Arvore PATRICIA.
- OPCAO 3: Imprime no terminal os indices invertidos (ordem alfabetica)
  e suas respectivas listas de ocorrencias (<qtde, idDoc>).
- OPCAO 4: Realiza a busca por um ou mais termos. O sistema calculara
  a relevancia usando a formula r(i) (TF-IDF) e mostrara os documentos 
  em ordem decrescente de relevancia.
- OPCAO 0: Encerra o programa e libera a memoria alocada.

6. LIMPEZA DOS ARQUIVOS COMPILADOS
Para apagar os arquivos objetos (.o) e o executavel gerados apos o 
uso, digite no terminal:
  make clean
